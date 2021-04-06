#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__

#include "Module.h"
#include "GuiControl.h"

#include "Animation.h"
#include "List.h"

#define MAGENTA {190,0,0,150}
#define RED {255,0,0,255}
#define SOFT_RED {240,80,0,255}

struct SDL_Texture;

class GuiManager : public Module
{
public:

	GuiManager();

	virtual ~GuiManager();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	GuiControl* CreateGuiControl(GuiControlType type);
	void DestroyGuiControl(GuiControl* entity);

	void AddGuiControl(GuiControl* entity);

	void DrawCursor();

	void DrawPlayerLifeBar(int life, int maxLife, int x, int y);

	void DrawEnemyLifeBar(int life, int maxLife, int x, int y);

	void BlinkLifeBar(int life,SDL_Color color1 , SDL_Color color2);

	void DrawCombatInterface();
	
	void DrawCombatButtons();

public:

	List<GuiControl*> controls;
	bool debugGui = false;

public: //Textures
	SDL_Texture* buttonSpriteSheet = nullptr;
	SDL_Texture* checkBoxSpriteSheet = nullptr;
	SDL_Texture* sliderSpriteSheet = nullptr;

public: //Others

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;
	bool doLogic = false;
	
private:
	SDL_Texture* cursorTexture;

	Animation* currentCursorAnim;
	Animation idleCursorAnim;
	Animation clickCursorAnim;
private:
	SDL_Rect maxLifeBar;
	SDL_Rect lifeBar;
public:
	SDL_Texture* GuiTexture;
	
private:
	SDL_Texture* faceAnimationsTexture;

	Animation* currentPlayerFaceAnim;
	Animation idleFaceAnim;
	Animation hurtFaceAnim;
public:
	SDL_Rect buttonPrefab = { 200,580,161,62 };
	int margin = 108;
	int padding = 26;

private:
	int secondsCounter;
	int frameCounter;
};

#endif // __GUIMANAGER_H__

//TEXT IMPLEMENTATION

//if (testText == nullptr)
//{
//	testText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
//	testText->bounds = { 0, 0, 105, 27 };
//	testText->SetTextFont(app->fontTTF->defaultFont);
//	testText->SetString("TEST");
//}
//testText->Draw();

//BUTTON IMPLEMENTATION

//if (testButton == nullptr)
//{
//	testButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
//	testButton->bounds = { 0, 0, 105, 27 };
//	testButton->text = "testButton";
//	testButton->SetObserver(this);
//}
//testButton->Update(0.0f);
//testButton->Draw();

//CHECKBOX IMPLEMENTATION

//if (testCheckBox == nullptr)
//{
//	testCheckBox = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX);
//	testCheckBox->bounds = { 0, 0, 105, 27 };
//	testCheckBox->text = "testCheckBox";
//	testCheckBox->SetObserver(this);
//}
//testCheckBox->Update(0.0f);
//testCheckBox->Draw();

//SLIDER IMPLEMENTATION

//if (testSlider == nullptr)
//{
//	testSlider = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER);
//	testSlider->bounds = { 0, 0, 100, 20 };
//  testSlider->SetSlider({testSlider->bounds.x, testSlider->bounds.y, 20, 20});
//	testSlider->text = "testSlider";
//  testSlider->SetMaxValue(100);
//  testSlider->SetMinValue(0);
//	testSlider->SetObserver(this);
//}
//testSlider->Update(0.0f);
//testSlider->Draw();
