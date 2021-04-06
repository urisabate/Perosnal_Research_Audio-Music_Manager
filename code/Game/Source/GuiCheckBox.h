#ifndef __GUICHECKBOX_H__
#define __GUICHECKBOX_H__

#include "App.h"
#include "GuiControl.h"

#include "Point.h"

class GuiCheckBox : public GuiControl
{
public:

    GuiCheckBox(uint32 id, SDL_Rect bounds, const char *text);
    virtual ~GuiCheckBox();

    bool Update(float dt);
    bool Draw();

    bool GetCheckedState() const
    {
        return checked;
    }

private:

    bool checked;
    const SDL_Rect Normal = { 0, 0, 0, 0};
    const SDL_Rect NormalC = { 0, 0, 0, 0 };
    const SDL_Rect Focused = { 0, 0, 0, 0 };
    const SDL_Rect FocusedC = { 0, 0, 0, 0 };
};

#endif // __GUICHECKBOX_H__
