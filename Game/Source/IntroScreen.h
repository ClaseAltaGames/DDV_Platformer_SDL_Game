#pragma once

#include "Module.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;


class IntroScreen : public Module
{
public:

    IntroScreen(bool startEnabled = true);
    virtual ~IntroScreen();

    bool Start();

    bool Update(float dt);

    bool PostUpdate();

    bool CleanUp();

public:

    SDL_Texture* introScreenTex;

private:
    int introSound;
};