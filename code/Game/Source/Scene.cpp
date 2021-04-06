#include "App.h"
#include "Scene.h"

#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Pathfinding.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "Combat.h"
#include "DialogueManager.h"

#include "GuiManager.h"
#include "GuiString.h"
#include "GuiControl.h"
#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"
#include "FontTTF.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{

}

Scene::~Scene()
{}

bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

bool Scene::Start()
{
	player1 = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);

	map = new Map();
	combatScene = new Combat();

	enviroment = GRASSY_LANDS;

	SetScene(LOGO_SCENE);

	app->entityManager->CreateEntity(EntityType::ENEMY);
	app->entityManager->CreateEntity(EntityType::ENEMY);
	app->entityManager->CreateEntity(EntityType::ENEMY);

	//ENEMY SET                                          ENEMY CLASS           ---------------------RECT-------------------     LVL EXP  HP STR DEF VEL
	app->entityManager->enemies.start->data->SetUp(EnemyClass::SMALL_WOLF, { INIT_SMALLWOLF_POSX, INIT_SMALLWOLF_POSY, 86, 44 }, 2, 200, 30, 30, 10, 20);
	app->entityManager->enemies.start->next->data->SetUp(EnemyClass::BIRD, { INIT_BIRD_POSX, INIT_BIRD_POSY, 40, 75 }, 2, 200, 30, 30, 10, 20);
	app->entityManager->enemies.start->next->next->data->SetUp(EnemyClass::MANTIS, { INIT_MANTIS_POSX, INIT_MANTIS_POSY, 56, 75 }, 2, 200, 30, 30, 10, 20);

	splitButton->state == GuiControlState::LOCKED;

	return true;
}

bool Scene::PreUpdate()
{
	/*
	// L12b: Debug pathfing
	static iPoint origin;
	static bool originSelected = false;

	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);
	iPoint p = app->render->ScreenToWorld(mouseX, mouseY);
	p = app->map->WorldToMap(p.x, p.y);

	if(app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if(originSelected == true)
		{
			app->pathFinding->CreatePath(origin, p);
			originSelected = false;
		}
		else
		{
			origin = p;
			originSelected = true;
		}
	}
	*/
	return true;
}

bool Scene::Update(float dt)
{
	// L12b: Debug pathfinding
	/*
	app->input->GetMousePosition(mouseX, mouseY);
	iPoint p = app->render->ScreenToWorld(mouseX, mouseY);
	p = app->map->WorldToMap(p.x, p.y);
	p = app->map->MapToWorld(p.x, p.y);

	const DynArray<iPoint>* path = app->pathFinding->GetLastPath();

	for(uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		app->render->DrawTexture(debugTex, pos.x, pos.y);
	}
	*/

	if (currScene == LOGO_SCENE) UpdateLogoScene();
	else if (currScene == MAIN_MENU) UpdateMainMenu();
	else if (currScene == COMBAT) UpdateCombat();
	else if (currScene == VILLAGE) UpdateVillage();
	else if (currScene == SOUND_TEST) UpdateSoundTest();

	return true;
}

bool Scene::PostUpdate()
{
	bool ret = true;

	app->win->FullScreenLogic();

	return ret;
}

bool Scene::CleanUp()
{
	LOG("Freeing scene");

	if (currScene == COMBAT) combatScene->Restart();

	return true;
}

// SCENE MANAGER

void Scene::SetScene(Scenes scene)
{
	CleanUp();

	prevScene = currScene;
	currScene = scene;

	if (scene == LOGO_SCENE) SetLogoScene();
	else if (scene == MAIN_MENU) SetMainMenu();
	//else if (scene == COMBAT) SetCombat();
	else if (scene == VILLAGE) SetVillage();
	else if (scene == SOUND_TEST) SetSoundTest();
}

void Scene::SetScene(Scenes scene, Enemy* enemy)
{
	prevScene = currScene;
	currScene = scene;

	CleanUp();

	if (scene == LOGO_SCENE) SetLogoScene();
	else if (scene == MAIN_MENU) SetMainMenu();
	else if (scene == COMBAT) SetCombat(enemy);
	else if (scene == VILLAGE) SetVillage();
	else if (scene == SOUND_TEST) SetSoundTest();
}

void Scene::SetLogoScene()
{

}

void Scene::SetMainMenu()
{

}

void Scene::SetCombat(Enemy* enemySet)
{
	combatScene->enemy = enemySet;
	combatScene->Start();

	app->guiManager->debugGui = true;

	SDL_Rect buttonPrefab = app->guiManager->buttonPrefab;
	
	if (attackButton == nullptr)
	{
		attackButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		attackButton->bounds = { app->guiManager->margin ,buttonPrefab.y,buttonPrefab.w,buttonPrefab.h };
		attackButton->text = "AttackButton";
		attackButton->SetObserver(this);
	}

	if (moveButton == nullptr)
	{
		moveButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		moveButton->bounds = { app->guiManager->margin + ((buttonPrefab.x + app->guiManager->padding)* 1),buttonPrefab.y,buttonPrefab.w,buttonPrefab.h };
		moveButton->text = "MoveButton";
		moveButton->SetObserver(this);
	}

	if (itemButton == nullptr)
	{
		itemButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		itemButton->bounds = { app->guiManager->margin + ((buttonPrefab.x + app->guiManager->padding) * 2),buttonPrefab.y,buttonPrefab.w,buttonPrefab.h };
		itemButton->text = "ItemButton";
		itemButton->SetObserver(this);
	}

	if (scapeButton == nullptr)
	{
		scapeButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		scapeButton->bounds = { app->guiManager->margin + ((buttonPrefab.x + app->guiManager->padding) * 3),buttonPrefab.y,buttonPrefab.w,buttonPrefab.h };
		scapeButton->text = "ScapeButton";
		scapeButton->SetObserver(this);
	}

	if (splitButton == nullptr)
	{
		splitButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
		splitButton->bounds = { app->guiManager->margin + ((buttonPrefab.x + app->guiManager->padding) * 4),buttonPrefab.y,buttonPrefab.w,buttonPrefab.h };
		splitButton->text = "SplitButton";
		splitButton->SetObserver(this);
	}
}

void Scene::SetVillage()
{
	map->Load("SplitDuty1.tmx");
}

void Scene::SetSoundTest()
{
	uint w, h;
	app->win->GetWindowSize(w, h);

	enemyPos = { int(w / 2), int(h / 2) };

	playerPos = SDL_Rect({ 100, 100 , 30, 30 });
}

void Scene::UpdateLogoScene()
{
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) SetScene(MAIN_MENU);
}

void Scene::UpdateMainMenu()
{
	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) SetScene(COMBAT, (Enemy*)app->entityManager->enemies.start->data);
	if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) SetScene(COMBAT, (Enemy*)app->entityManager->enemies.start->next->data);
	if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) SetScene(COMBAT, (Enemy*)app->entityManager->enemies.start->next->next->data);
	if (app->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN) SetScene(VILLAGE);

	if (app->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
	{
		app->dialogueManager->StartDialogue(1); 
		app->guiManager->debugGui = true;
	}
	if (app->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN) SetScene(SOUND_TEST);
}

void Scene::UpdateCombat()
{
	combatScene->Update();

	combatScene->Draw();

	app->guiManager->Update(1.0f);

	//app->guiManager->DrawCombatInterface();

	if (combatScene->playerEscape) SetScene(MAIN_MENU);

	RestartPressState();

	app->guiManager->DrawCombatInterface();

	DebugSteps();
}

void Scene::UpdateVillage()
{
	map->Draw();

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) SetScene(MAIN_MENU);
}

void Scene::UpdateSoundTest()
{
	app->render->DrawCircle(enemyPos.x, enemyPos.y, 30, SDL_Color(RED));
	app->render->DrawRectangle(playerPos, SDL_Color(RED));

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		playerPos.x += 5;
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		playerPos.x -= 5;
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		playerPos.y -= 5;
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		playerPos.y += 5;

	uint w, h;
	app->win->GetWindowSize(w, h);

	if ((playerPos.x + playerPos.w) > w)
		playerPos.x = w - playerPos.w;
	if ((playerPos.y + playerPos.h) > h)
		playerPos.y = h - playerPos.h;
	if (playerPos.x < 0)
		playerPos.x = 0;
	if (playerPos.y < 0)
		playerPos.y = 0;

	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		app->audio->PlayFxOnChannel(1, app->audio->GetAngle({ playerPos.x, playerPos.y }, enemyPos),
									app->audio->GetVolumeFromDistance({ playerPos.x, playerPos.y }, enemyPos));
}

// GUI CONTROLS

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (currScene)
	{
	case LOGO_SCENE:
		break;

	case MAIN_MENU:
		break;

	case COMBAT:

		if (strcmp(control->text.GetString(), "AttackButton") == 0) attackPressed = true;
		else if (strcmp(control->text.GetString(), "MoveButton") == 0) movePressed = true;
		else if (strcmp(control->text.GetString(), "ItemButton") == 0) itemPressed = true;
		else if (strcmp(control->text.GetString(), "ScapeButton") == 0) scapePressed = true;
		else if (strcmp(control->text.GetString(), "SplitButton") == 0) splitPressed = true;

		break;
	}

	return true;
}

void Scene::RestartPressState()
{
	attackPressed = false;
	movePressed = false;
	itemPressed = false;
	scapePressed = false;
	splitPressed = false;
}

// Debug functions (future in debug module)

void Scene::DebugSteps()
{
	app->render->DrawLine(249, 498, 249, 510, {255, 255, 255, 255});
	app->render->DrawLine(419, 498, 419, 510, {255, 255, 255, 255});
	app->render->DrawLine(589, 498, 589, 510, {255, 255, 255, 255});
	app->render->DrawLine(759, 498, 759, 510, {255, 255, 255, 255});
	app->render->DrawLine(1031, 498, 1031, 510, {255, 0, 0, 255});
}