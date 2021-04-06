#ifndef _DIALOGUENODE_H__
#define _DIALOGUENODE_H__

#include "Dialogue.h"

class DialogueNode : public Dialogue
{
public:
	//default constructor
	DialogueNode() {};

	DialogueNode(SString text, int id) : nodeText(text), nodeID(id),
				 optionsActive(false), nodeTexture(nullptr) {};

	virtual ~DialogueNode() {};

public:
	int						nodeID;
	bool					optionsActive = true;
		
	SString					nodeText;
	List<DialogueOption*>	optionsList;

	SDL_Texture*			nodeTexture;
};
#endif // __DIALOGUENODE_H__
