#pragma once

#include "Module.h"
#include "Animation.h"
#include "Path.h"
#include "Point.h"
#include "p2point.h"

#include "SDL/include/SDL.h"

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

    Animation* introScreenAnim;

    Path* introScreenPath;

private:

};