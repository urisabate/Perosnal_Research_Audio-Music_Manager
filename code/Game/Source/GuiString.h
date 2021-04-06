#ifndef _GUISTRING_H_
#define _GUISTRING_H_

#include "GuiControl.h"
#include "App.h"
#include "FontTTF.h"

class SString;
struct _TTF_Font;
struct SDL_Texture;

class GuiString : public GuiControl
{
public:

	GuiString(uint32 id, SDL_Rect bounds, const char* text, _TTF_Font* textFont);
	GuiString();

	virtual ~GuiString();

public:

	void Draw();
	void SetString(char* newText);
	void SetTextFont(_TTF_Font* textFont);

private:
	_TTF_Font* textFont = nullptr;
	SDL_Texture* textTexture = nullptr;
};

#endif // !_GUISTRING_H_
