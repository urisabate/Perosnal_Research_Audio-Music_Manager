#ifndef __SCENE_H__
#define __SCENE_H__

#include "SDL/include/SDL.h"

#include "Module.h"

class GuiButton;
class GuiSlider;
class GuiCheckBox;
class GuiString;
struct SDL_Texture;
struct SDL_Rect;
class Enemy;
class Player;
class Combat;
class Map;

enum Scenes
{
	NONE = -1,
	LOGO_SCENE,
	MAIN_MENU,
	COMBAT,
	VILLAGE,
	SOUND_TEST
};

enum Environments
{
	GRASSY_LANDS,
	AUTUMM_FALLS,
	MOSSY_LANDS
};

class Scene : public Module
{
public:

	Scene();

	virtual ~Scene();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	Scenes GetCurrScene() const
	{
		return currScene;
	}

public: //Combat Buttons

	void RestartPressState();

	GuiButton* attackButton = nullptr;
	bool attackPressed = false;

	GuiButton* moveButton = nullptr;
	bool movePressed = false;

	GuiButton* itemButton = nullptr;
	bool itemPressed = false;

	GuiButton* scapeButton = nullptr;
	bool scapePressed = false;

	GuiButton* splitButton = nullptr;
	bool splitPressed = false;

public:
	friend class Combat;
	friend class Enemy;
	friend class Player;

	Player* player1;
	Combat* combatScene;
	Map* map;
	Environments enviroment;

	//Audio Testing
	iPoint enemyPos;
	SDL_Rect playerPos;

private: //Scene Manager
	Scenes currScene = NONE;
	Scenes prevScene = NONE;

	void SetScene(Scenes scene);
	void SetScene(Scenes scene, Enemy* enemy);

	//Setters
	void SetLogoScene();
	void SetMainMenu();
	void SetCombat(Enemy* enemy);
	void SetVillage();
	void SetSoundTest();

	//Updaters
	void UpdateLogoScene();
	void UpdateMainMenu();
	void UpdateCombat();
	void UpdateVillage();
	void UpdateSoundTest();

private: //debug
	void DebugSteps(); // Future Debug Module implementation

private:
	bool OnGuiMouseClickEvent(GuiControl* control);
};

#endif // __SCENE_H__