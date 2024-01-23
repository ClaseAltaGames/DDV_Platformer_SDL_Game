#pragma once

#include "Module.h"
#include "SDL/include/SDL.h"
#include "Window.h"
#include "GuiControlButton.h"
#include "GuiManager.h"
#include "GuiControl.h"
#include "GuiControlSlider.h"
#include "GuiControlCheckBox.h"

struct SDL_Texture;


class PauseScreen : public Module
{
public:

    PauseScreen(bool startEnabled = true);
    virtual ~PauseScreen();

    bool Start();

    bool Update(float dt);

    bool PostUpdate();

    bool CleanUp();

public:

    SDL_Texture* pauseScreenTex;

    GuiControl* sliderFx;
    GuiControlSlider* sliderMusic;

    GuiControlCheckBox* checkBoxFullscreen;
    GuiControlCheckBox* checkBoxVSync;

    bool completedFxAvailable = false;

private:


};