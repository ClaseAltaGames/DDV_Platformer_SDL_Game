#pragma once

#include "Module.h"
#include "SDL/include/SDL.h"
#include "Window.h"
#include "GuiControlButton.h"
#include "GuiManager.h"
#include "GuiControl.h"

struct SDL_Texture;


class Credits : public Module
{
public:

    Credits(bool startEnabled = true);
    virtual ~Credits();

    bool Start();

    bool Update(float dt);

    bool PostUpdate();

    bool CleanUp();

    void CreateCreditsButtons();

public:

    SDL_Texture* creditsTexture;
    int titleSound;

    //gui list
    List<GuiControl*> creditsButtons;

private:


};