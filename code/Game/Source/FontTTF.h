#ifndef _FONTTTF_H_
#define _FONTTTF_H_

#include "Module.h"
#include "SDL/include/SDL_pixels.h"
#include "List.h"

#define DEFAULT_FONT "Assets/Fonts/kenvector_future_thin.ttf"
#define DEFAULT_FONT_SIZE 12

struct SDL_Texture;
struct _TTF_Font;

class FontTTF : public Module
{
public:
	FontTTF();

	virtual ~FontTTF();

	bool Awake(pugi::xml_node&);

	bool CleanUp();

	_TTF_Font* const Load(const char* path, int size = 12);

	SDL_Texture* Print(const char* text, SDL_Color color = { 255, 255, 255, 255 }, _TTF_Font* font = NULL, SDL_Texture* tex = nullptr);

	bool CalcSize(const char* text, int& width, int& height, _TTF_Font* font = NULL) const;

public:

	List<_TTF_Font*> fonts;
	_TTF_Font* defaultFont = nullptr;
};
#endif // !_FONTTTF_H_
