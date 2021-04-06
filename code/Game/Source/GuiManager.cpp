#include "App.h"
#include "Textures.h"

#include "GuiManager.h"
#include "Player.h"
#include "Combat.h"

#include "Log.h"

#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"
#include "GuiString.h"
#include "FontTTF.h"



GuiControl* GuiManager::CreateGuiControl(GuiControlType type)
{
	GuiControl* control = nullptr;

	switch (type)
	{
	case GuiControlType::BUTTON: control = new GuiButton(NULL, { 0, 0, 0, 0 }, "0"); break;
	case GuiControlType::CHECKBOX: control = new GuiCheckBox(NULL, { 0, 0, 0, 0 }, "0"); break;
	case GuiControlType::SLIDER: control = new GuiSlider(NULL, { 0, 0, 0, 0 }, "0"); break;
	case GuiControlType::TEXT: control = new GuiString(); break;
	default: break;
	}

	if (control != nullptr) controls.Add(control);

	

	return control;
}

void GuiManager::DestroyGuiControl(GuiControl* entity)
{
	delete entity;
	entity = nullptr;
}

GuiManager::GuiManager()
{
}

GuiManager::~GuiManager()
{
}

bool GuiManager::Awake(pugi::xml_node&)
{
	return true;
}

bool GuiManager::Start()
{
	secondsCounter = 0;
	frameCounter = 0;

	buttonSpriteSheet = nullptr;
	checkBoxSpriteSheet = nullptr;
	sliderSpriteSheet = nullptr;

	cursorTexture = app->tex->Load("Assets/Textures/UI/grab_hand.png");
	GuiTexture = app->tex->Load("Assets/Textures/UI/GUI.png");
	faceAnimationsTexture = app->tex->Load("Assets/Textures/UI/face_animations.png");

	idleCursorAnim.PushBack({ 0,0,30,30});

	//clickCursorAnim.PushBack({ 0,0,30,30 });
	clickCursorAnim.PushBack({ 32,0,30,30 });
	clickCursorAnim.PushBack({ 64,0,30,30 });
	clickCursorAnim.PushBack({ 32,0,30,30 });
	clickCursorAnim.PushBack({ 0,0,30,30 });
	clickCursorAnim.loop = false;
	clickCursorAnim.speed = 0.5f;

	currentCursorAnim = &idleCursorAnim;

	//Buttons
	

	//face
	idleFaceAnim.PushBack({ 0,0,70,69 });
	idleFaceAnim.PushBack({ 70,0,70,69 });


	currentPlayerFaceAnim = &idleFaceAnim;


	return true;
}

bool GuiManager::Update(float dt)
{
	frameCounter++;
	if (frameCounter % 25 == 0)
		secondsCounter++;

	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	//Activate Debug Draw
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) debugGui = !debugGui;

	//UpdateAll(dt, doLogic);

	if (doLogic == true)
	{
		accumulatedTime = 0.0f;
		doLogic = false;
	}

	
	

	

	return true;
}

void GuiManager::DrawCursor()
{
	currentCursorAnim->Update(1.0f);

	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);

	app->render->DrawTexture(cursorTexture, mouseX, mouseY, &currentCursorAnim->GetCurrentFrame());

	if(app->input->GetMouseButtonDown(1) && currentCursorAnim != &clickCursorAnim)
	{
		currentCursorAnim = &clickCursorAnim;
		LOG("Clicked");
	}
	if (currentCursorAnim->HasFinished() && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP )
	{
		currentCursorAnim = &idleCursorAnim;
		clickCursorAnim.Reset();
		LOG("change cursor anim");
		
	}
	


}

void GuiManager::DrawCombatButtons()
{
	app->scene->attackButton->Draw();
	app->scene->moveButton->Draw();
	app->scene->itemButton->Draw();
	app->scene->scapeButton->Draw();
	app->scene->splitButton->Draw();
}

void GuiManager::DrawPlayerLifeBar(int life,int maxLife,int x,int y)
{
	int size = 4;
	int thickness = 20;
	maxLifeBar = { x,y,maxLife*size,thickness };
	app->render->DrawRectangle(maxLifeBar, MAGENTA);
	lifeBar = { x,y,life*size,thickness };
	
	//if life is critical blinks
	if (life <= maxLife / 3)
	{
		BlinkLifeBar(life, RED, SOFT_RED);
		
	}
	else
		app->render->DrawRectangle(lifeBar, RED);
}

void GuiManager::DrawEnemyLifeBar(int life, int maxLife, int x, int y)
{
	int size = 4;
	int thickness = 20;
	int offset = maxLife * size;
	app->render->DrawRectangle({x - offset,y,maxLife*size,thickness }, MAGENTA);
	lifeBar = { x-life*4,y,life * size,thickness };

	//if life is critical blinks
	if (life <= maxLife / 3)
	{
		BlinkLifeBar(life, RED, SOFT_RED);

	}
	else
		app->render->DrawRectangle(lifeBar, RED);
}

void GuiManager::BlinkLifeBar(int life, SDL_Color color1, SDL_Color color2)
{
	
	if(secondsCounter % 2 ==0)
		app->render->DrawRectangle(lifeBar, color1);
	else
		app->render->DrawRectangle(lifeBar, color2);
}

void GuiManager::DrawCombatInterface()
{
	
	const SDL_Rect guiRect = { 0,0,1280,720 };
	app->render->DrawTexture(GuiTexture, 0, 0, &guiRect);

	
	//const SDL_Rect faceRect = { 0,0,70,69 };
	//app->render->DrawTexture(faceAnimationsTexture, 71, 27, &faceRect);

	

	app->guiManager->DrawPlayerLifeBar(app->scene->player1->health, app->scene->player1->maxHealth, 182, 30);

	app->guiManager->DrawEnemyLifeBar(30, 35, 1086, 30);

	DrawCombatButtons();

	app->guiManager->DrawCursor();
}

bool GuiManager::CleanUp()
{
	return true;
}

