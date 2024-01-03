#pragma once

#include "Module.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;


class TitleScreen : public Module
{
public:

    TitleScreen(bool startEnabled = true);
    virtual ~TitleScreen();

    bool Start();

    bool Update(float dt);

    bool PostUpdate();

    bool CleanUp();

public:

    SDL_Texture* titleScreenTex;

private:

};