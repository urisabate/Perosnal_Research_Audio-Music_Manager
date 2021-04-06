#include "GuiString.h"
#include "Textures.h"

GuiString::GuiString(uint32 id, SDL_Rect bounds, const char* text, _TTF_Font* textFont) : GuiControl(GuiControlType::TEXT, id)
{
	this->bounds = bounds;
	this->text = text;
	this->textFont = textFont;
	textTexture = app->fontTTF->Print(this->text.GetString(), { 255,255,255,255 }, textFont);
}

GuiString::GuiString() : GuiControl(GuiControlType::TEXT, id), textFont(app->fontTTF->defaultFont)
{
	textTexture = app->fontTTF->Print(this->text.GetString(), { 255,255,255,255 }, textFont);
}

GuiString::~GuiString()
{
	app->tex->UnLoad(textTexture);
	delete &text;
}

void GuiString::Draw()
{
	app->render->DrawTexture(textTexture, bounds.x - app->render->camera.x, bounds.y - app->render->camera.y);
}

void GuiString::SetString(char* newText)
{
	text = newText;
	textTexture = app->fontTTF->Print(text.GetString(), { 255,255,255,255 }, textFont, textTexture);
}

void GuiString::SetTextFont(_TTF_Font* textFont)
{
	this->textFont = textFont;
}