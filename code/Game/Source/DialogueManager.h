#ifndef __DIALOGUEMANAGER_H__
#define __DIALOGUEMANAGER_H__

#include "App.h"
#include "Module.h"
#include "Input.h"
#include "Dialogue.h"
#include "DialogueNode.h"
#include "DialogueOption.h"
#include "FontTTF.h"

#include "SDL/include/SDL_rect.h"
//#include "SDL/include/SDL.h"

#include "Log.h"
#include "List.h"

#define DIALOGUE_CONFIG_FILENAME		"Dialog_Config.xml"

struct SDL_Texture;
struct SDL_Color;
struct SDL_Rect;

class DialogueManager : public Module
{
public: //functions
	// Constructor and destructor //
	DialogueManager();
	virtual ~DialogueManager() {}
	////////////////////////////////
	// Flow Functions
	bool Awake(pugi::xml_node&);

	bool Start();

	//bool PreUpdate();

	bool Update(float dt);

	//bool PostUpdate();

	bool CleanUp();
	/////////////////////////////////	
	void StartDialogue(int dialogueID);

private: //functions

	//////CREATORS//////
	/*Dialogue CreateDialogue(pugi::xml_node&);*/
	DialogueOption* CreateOptions(pugi::xml_node& setter);
	DialogueNode* CreateNode(pugi::xml_node& setter);
	void CreateDialogue(pugi::xml_node& setter);
	void EndDialogue();

	void Draw();
	void Input();

	bool OnGuiMouseClickEvent(GuiControl* option);

	pugi::xml_node LoadDialogueConfig(pugi::xml_document&) const;
public: //variables

private: // variables

	Dialogue*			currentDialogue;
	List<Dialogue*>		dialoguesList;

	SDL_Color           black	=		{ 0  , 0  , 0  , 200 };
	SDL_Color           white	=		{ 255, 255, 255, 200 };
	SDL_Color           red		=		{ 255, 0  , 0  , 200 };
	SDL_Color           green	=		{ 0  , 255, 0  , 200 };
	SDL_Color           blue	=		{ 0  , 0  , 255, 200 };
};

#endif // __DIALOGUEMANAGER_H__
