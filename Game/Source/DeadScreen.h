#pragma once

#include "Module.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class DeadScreen : public Module
{
public:

    DeadScreen(bool startEnabled = true);
    virtual ~DeadScreen();

    bool Start();

    bool Update(float dt);

    bool PostUpdate();

    bool CleanUp();

public:

    SDL_Texture* deadScreenTex;
    int deadSound;

private:


};