#include "App.h"
#include "GuiCheckBox.h"
#include "GuiManager.h"
#include "Audio.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::CHECKBOX, id)
{
    this->bounds = bounds;
    this->text = text;
}

GuiCheckBox::~GuiCheckBox()
{
    delete &text;
}

bool GuiCheckBox::Update(float dt)
{
    if (state != GuiControlState::DISABLED)
    {
        int mouseX, mouseY;
        app->input->GetMousePosition(mouseX, mouseY);

        if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
            (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
        {
            if (state == GuiControlState::NORMAL)
            {
                //app->audio->PlayFx(focus mouse sound);
            }
            state = GuiControlState::FOCUSED;

            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
            }

            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
            {
                checked = !checked;
                //app->audio->PlayFx( Click sound);
                NotifyObserver();
            }
        }
        else state = GuiControlState::NORMAL;
    }

    return false;
}

bool GuiCheckBox::Draw()
{
    switch (state)
    {
    case GuiControlState::DISABLED:
    {
        if (checked) app->render->DrawRectangle(bounds, { 100, 100, 100, 255 });
        else app->render->DrawRectangle(bounds, { 100, 100, 100, 255 });

    } break;

    case GuiControlState::NORMAL:
    {
        if (checked) app->render->DrawTexture(app->guiManager->checkBoxSpriteSheet, bounds.x, bounds.y, &NormalC);
        else app->render->DrawTexture(app->guiManager->checkBoxSpriteSheet, bounds.x, bounds.y, &Normal);
    } break;

    case GuiControlState::FOCUSED:
    {
        if (checked) app->render->DrawTexture(app->guiManager->checkBoxSpriteSheet, bounds.x, bounds.y, &FocusedC);
        else app->render->DrawTexture(app->guiManager->checkBoxSpriteSheet, bounds.x, bounds.y, &Focused);
    }break;

    case GuiControlState::PRESSED:
    {
        if (checked) app->render->DrawTexture(app->guiManager->checkBoxSpriteSheet, bounds.x, bounds.y, &FocusedC);
        else app->render->DrawTexture(app->guiManager->checkBoxSpriteSheet, bounds.x, bounds.y, &Focused);
    } break;

    case GuiControlState::SELECTED: app->render->DrawRectangle(bounds, { 0, 255, 0, 255 });
        break;
    default:
        break;
    }

    if (app->guiManager->debugGui)
    {
        switch (state)
        {
        case GuiControlState::DISABLED: app->render->DrawRectangle(bounds, { 100, 100, 100, 80 });
            break;
        case GuiControlState::NORMAL: 
        {
            if (!checked) app->render->DrawRectangle(bounds, { 0, 255, 0, 80 });
            else if (checked) app->render->DrawRectangle(bounds, { 255, 0, 0, 80 });
        }
            break;
        case GuiControlState::FOCUSED: app->render->DrawRectangle(bounds, { 255, 255, 0, 80 });
            break;
        case GuiControlState::PRESSED: app->render->DrawRectangle(bounds, { 0, 255, 255, 80 });
            break;
        case GuiControlState::SELECTED: app->render->DrawRectangle(bounds, { 0, 255, 0, 80 });
            break;
        }
    }
    return false;
}
