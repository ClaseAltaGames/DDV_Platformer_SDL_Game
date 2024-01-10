#pragma once

#include "Module.h"
#include "SDL/include/SDL.h"
#include "Window.h"
#include "GuiControlButton.h"
#include "GuiManager.h"
#include "GuiControl.h"

struct SDL_Texture;


class LevelCompletedScreen : public Module
{
public:

    LevelCompletedScreen(bool startEnabled = true);
    virtual ~LevelCompletedScreen();

    bool Start();

    bool Update(float dt);

    bool PostUpdate();

    bool CleanUp();

public:

    SDL_Texture* completedScreenTex;
    int completedSound;

    GuiControlButton* completedButton;

    bool completedFxAvailable = false;

private:


};