#include "DialogueManager.h"
#include "GuiButton.h"
#include "Textures.h"
#include "GuiManager.h"


DialogueManager::DialogueManager()
{
	name.Create("dialoguemanager");
	active = true;
	currentDialogue = nullptr;
	dialoguesList.Clear();
}

bool DialogueManager::Awake(pugi::xml_node&)
{
	bool ret = true;
	// HARDCODED DIALOGUE
	pugi::xml_document dConfigFile;
	pugi::xml_node configDial;

	configDial = LoadDialogueConfig(dConfigFile);

	if (configDial.empty() == false)
	{
		pugi::xml_node dSetUp = configDial.child("Dialogs");
		CreateDialogue(dSetUp);
	}
	return ret;
}

bool DialogueManager::Start()
{	
	return true;
}

bool DialogueManager::Update(float dt)
{
	bool ret = true;
	if (currentDialogue != nullptr)
	{
		Draw();
		Input();
	}
	return ret;
}

bool DialogueManager::CleanUp()
{
	for (ListItem<Dialogue*>* dialogueItem = dialoguesList.start;
		dialogueItem != nullptr; dialogueItem = dialogueItem->next)
	{
		for (ListItem<DialogueNode*>* nodeItem = dialogueItem->data->nodeList.start;
			nodeItem != nullptr; nodeItem = nodeItem->next)
		{
			if (nodeItem->data->optionsList.Count() != 0)
			{
				nodeItem->data->optionsList.Clear();
			}
			else continue;
		}
		dialogueItem->data->nodeList.Clear();
	}
	dialoguesList.Clear();
	return true;
}

DialogueOption* DialogueManager::CreateOptions(pugi::xml_node& setter)
{	
	DialogueOption* option = new DialogueOption(setter.attribute("NextNode").as_int(),
		setter.attribute("ReturnCode").as_int(),SString(setter.child_value()));

	return option;
}

DialogueNode* DialogueManager::CreateNode(pugi::xml_node& setter)
{
	pugi::xml_node nd = setter.child("NPCTalk");

	DialogueNode* node = new DialogueNode(SString(nd.child_value()), setter.attribute("ID").as_int());

	for (pugi::xml_node optionSetter = setter.child("Option"); optionSetter != nullptr;
		optionSetter = optionSetter.next_sibling("Option"))
	{	
		node->optionsList.Add(CreateOptions(optionSetter));
	}
	return node;
}

void DialogueManager::CreateDialogue(pugi::xml_node& setter)
{
	for (pugi::xml_node dlg = setter.child("Dialog"); dlg != nullptr; dlg = dlg.next_sibling("Dialog"))
	{	
		Dialogue* dialogue = new Dialogue(dlg.attribute("ID").as_int());	
		LOG("MY DIALOGUE TREE NUMBER %d", dialogue->dialogueID);

		for (pugi::xml_node nodeSetter = dlg.child("Node"); nodeSetter != nullptr; 
			 nodeSetter = nodeSetter.next_sibling("Node"))
		{
			dialogue->nodeList.Add(CreateNode(nodeSetter));
		}
		dialogue->AssignOptions();
		dialogue->currentNode = dialogue->nodeList.start->data;
	
		dialoguesList.Add(dialogue);
	}
}

void DialogueManager::EndDialogue()
{
	if (currentDialogue != nullptr)
	{
		if(currentDialogue->currentNode->optionsActive == true)
			currentDialogue->currentNode->optionsActive = false;

		currentDialogue->currentNode = currentDialogue->nodeList.start->data;
		for (ListItem<DialogueNode*>* itemNode = currentDialogue->nodeList.start;
			itemNode != nullptr; itemNode = itemNode->next)
		{
			if (itemNode->data->optionsList.Count() != 0)
			{
				for (ListItem<DialogueOption*>* itemOption = itemNode->data->optionsList.start;
					itemOption != nullptr; itemOption = itemOption->next)
				{
					app->tex->UnLoad(itemOption->data->optionTexture);
					if (itemOption->data->optionTexture != nullptr) LOG("OPTION TEXTURE NOT DELETED");
				}
			}		
			app->tex->UnLoad(itemNode->data->nodeTexture);
			if (itemNode->data->nodeTexture != nullptr) LOG("NODE TEXTURE NOT DELETED");
		}
		currentDialogue = nullptr;
	}
}

void DialogueManager::StartDialogue(int dialogueID)
{
	if (currentDialogue == nullptr)
	{
		currentDialogue = dialoguesList.At(--dialogueID)->data;
		currentDialogue->currentNode = currentDialogue->nodeList.start->data;

		for (ListItem<DialogueNode*>* itemNode = currentDialogue->nodeList.start;
			itemNode != nullptr; itemNode = itemNode->next)
		{
			itemNode->data->nodeTexture = app->fontTTF->Print(itemNode->data->nodeText.GetString(), white,
										  app->fontTTF->defaultFont);

			if (itemNode->data->nodeTexture == nullptr) LOG("NODE TEXTURE NOT LOADED");

			if (itemNode->data->optionsList.Count() != 0)
			{
				for (ListItem<DialogueOption*>* itemOption = itemNode->data->optionsList.start;
					itemOption != nullptr; itemOption = itemOption->next)
				{
					itemOption->data->optionTexture = app->fontTTF->Print(itemOption->data->optionText.GetString(), 
													  white, app->fontTTF->defaultFont);

					if (itemOption->data->optionTexture == nullptr) LOG("OPTION TEXTURE NOT LOADED");
				}
			}
			else continue;
		}
	}
}

void DialogueManager::Draw()
{
	SDL_Rect tempRect{ 0,0,0,0 };
	iPoint nodePos{ 100,100 };
	int offset = 15;

	app->fontTTF->CalcSize(currentDialogue->currentNode->nodeText.GetString(),
						   tempRect.w, tempRect.h, app->fontTTF->defaultFont);

	SDL_Rect nodeChart{ nodePos.x,nodePos.y, tempRect.w + (2 * offset), tempRect.h + (2 * offset) };

	//DRAWING NODE
	app->render->DrawRectangle(nodeChart, red);
	app->render->DrawTexture(currentDialogue->currentNode->nodeTexture,
							 nodeChart.x + offset, nodeChart.y + offset, &tempRect);

	if (currentDialogue->currentNode->optionsList.Count() != 0)
	{
		currentDialogue->currentNode->optionsActive = true;
		int i = 0;
		iPoint optionPos{ 100, 150 };

		for (ListItem<DialogueOption*>* itemOption = currentDialogue->currentNode->optionsList.start;
			itemOption != nullptr; itemOption = itemOption->next)
		{
			itemOption->data->optionButton = new GuiButton(itemOption->data->nodeID, { 0,0,0,0 }, 
														   itemOption->data->optionText.GetString());
			//DRAWING OPTIONS
			app->fontTTF->CalcSize(itemOption->data->optionText.GetString(), itemOption->data->optionButton->bounds.w, 
								   itemOption->data->optionButton->bounds.h, app->fontTTF->defaultFont);

			itemOption->data->optionButton->SetObserver(this);

			SDL_Rect optionChart{ 0,0,itemOption->data->optionButton->bounds.w, itemOption->data->optionButton->bounds.h };

			switch (i)
			{
			case 0:
				itemOption->data->optionButton->bounds.x = optionPos.x = 100;
				itemOption->data->optionButton->bounds.y = optionPos.y = 150;
				itemOption->data->optionButton->bounds.w += 15;
				itemOption->data->optionButton->bounds.h += 15; 

				itemOption->data->optionButton->Draw();
				app->render->DrawTexture(itemOption->data->optionTexture, (optionPos.x + offset), (optionPos.y + offset),
					                     &optionChart);
				break;
			case 1:
				itemOption->data->optionButton->bounds.x = optionPos.x = 100;
				itemOption->data->optionButton->bounds.y = optionPos.y = 200;
				itemOption->data->optionButton->bounds.w += 15;
				itemOption->data->optionButton->bounds.h += 15;

				itemOption->data->optionButton->Draw();
				app->render->DrawTexture(itemOption->data->optionTexture, (optionPos.x + offset), (optionPos.y + offset),
										 &optionChart);
				break;
			default:
				break;
			}
			i++;
		}
	}
}

void DialogueManager::Input()
{
	if (currentDialogue != nullptr && currentDialogue->currentNode->optionsActive)
	{
		for (ListItem<DialogueOption*>* itemOption = currentDialogue->currentNode->optionsList.start;
			itemOption != nullptr; itemOption = itemOption->next)
		{
			itemOption->data->optionButton->Update(.0f);
		}		
	}
	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) EndDialogue();
}

bool DialogueManager::OnGuiMouseClickEvent(GuiControl* option)
{
	bool ret = true;
	currentDialogue->currentNode->optionsActive = false;

	for (ListItem<DialogueOption*>* itemOption = currentDialogue->currentNode->optionsList.start; 
		 itemOption != nullptr; itemOption = itemOption->next)
	{
		//DELETE GUI OPTION BUTTONS HERE
		app->guiManager->DestroyGuiControl(itemOption->data->optionButton);

		if (strcmp(option->text.GetString(), itemOption->data->optionText.GetString()) == 0)
		{
			if(itemOption->data->nextNode != nullptr && itemOption->data->returnCode == 1)
			{
				currentDialogue->currentNode = itemOption->data->nextNode;
			}
			else if (itemOption->data->returnCode == 0)
			{
				//node with continue option
				//==> go to next node
			}
			else if (itemOption->data->returnCode == -1)
			{
				EndDialogue();
			}
		}
	}
	return ret;
}

pugi::xml_node DialogueManager::LoadDialogueConfig(pugi::xml_document& configFile) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = configFile.load_file(DIALOGUE_CONFIG_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", DIALOGUE_CONFIG_FILENAME, result.description());
	else ret = configFile.child("DialogSettup");

	return ret;
	
}