#ifndef __DIALOGUEOPTION_H__
#define __DIALOGUEOPTION_H__

#include "DialogueNode.h"
#include "GuiButton.h"

class DialogueOption : public DialogueNode
{
public:
	//default constructor
	DialogueOption() {};

	DialogueOption(int nextNode, int returnCode, SString text) : nextNodeID(nextNode), 
				   optionText(text), returnCode(returnCode), optionTexture(nullptr), optionButton(nullptr) {};

	virtual ~DialogueOption() {};

public: 

	int				nextNodeID;
	int				returnCode;

	GuiButton*		optionButton;
	SDL_Texture*	optionTexture;
	SDL_Rect		optionRect{ 0,0,0,0 };
	SString			optionText;	
	DialogueNode*	nextNode = nullptr;

};

#endif // __DIALOGUEOPTION_H__
