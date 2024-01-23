#include "GuiControlSlider.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Log.h"
#include "Window.h"

GuiControlSlider::GuiControlSlider(uint32 id, SDL_Rect bounds, int minValue, int maxValue, const char* text)
    : GuiControl(GuiControlType::SLIDER, id), minValue(minValue), maxValue(maxValue), value(minValue)
{
    this->bounds = bounds;
    this->text = text;

    canClick = true;
    drawBasic = false;
}

GuiControlSlider::~GuiControlSlider()
{

}

bool GuiControlSlider::Update(float dt)
{
    

    return false;
}

bool GuiControlSlider::PostUpdate()
{
    if (state != GuiControlState::DISABLED)
    {
        app->input->GetMousePosition(mouseX, mouseY);

        mouseX *= app->win->GetScale();
        mouseY *= app->win->GetScale();

        // Update the state based on the mouse position
        if (mouseX > bounds.x && mouseX < bounds.x + bounds.w && mouseY > bounds.y && mouseY < bounds.y + bounds.h)
        {
            state = GuiControlState::FOCUSED;

            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
            {
                state = GuiControlState::PRESSED;
            }
        }else{
            state = GuiControlState::NORMAL;
        }

        // Update the value if the mouse is pressed and the slider is focused or pressed
        if (state == GuiControlState::PRESSED || (state == GuiControlState::FOCUSED && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT))
        {
            // Calculate the new value based on the mouse position
            float normalizedX = static_cast<float>(mouseX - bounds.x) / static_cast<float>(bounds.w);
            value = static_cast<int>(minValue + normalizedX * (maxValue - minValue));

            // Ensure the value stays within the valid range
            value = std::min(std::max(value, minValue), maxValue);

            NotifyObserver();
        }

        // Draw the slider
        Draw();
    }

    return false;
}

void GuiControlSlider::Draw()
{
    app->render->DrawRectangle(bounds, 150, 150, 150, 255, true, false);

    // Calculate the position of the slider knob based on the current value
    float normalizedValue = static_cast<float>(value - minValue) / static_cast<float>(maxValue - minValue);
    int knobX = bounds.x + static_cast<int>(normalizedValue * bounds.w);

    // Draw the slider knob
    SDL_Rect knobRect = { knobX - 5, bounds.y - 5, 10, bounds.h + 10 };
    app->render->DrawRectangle(knobRect, 255, 0, 0, 255, true, false);

    // Draw the text label
    app->render->DrawText(text.GetString(), bounds.x, bounds.y - 20, bounds.w, 20);

}