#pragma once

#include "Module.h"
#include "SDL/include/SDL.h"
#include "Window.h"

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


    int wBt = 100;
    int hBt = 20;
    int posBtX = SCREEN_WIDTH / 2 - wBt / 2;
    int posBtY = SCREEN_HEIGHT / 2 - hBt / 2;


    bool completedFxAvailable = false;

private:


};