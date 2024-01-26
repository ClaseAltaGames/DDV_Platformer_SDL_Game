#pragma once

#include "Module.h"
#include "SDL/include/SDL.h"
#include "Window.h"
#include "GuiControlButton.h"
#include "GuiManager.h"
#include "GuiControl.h"

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

    void CreateTitleButtons();

public:

    SDL_Texture* titleScreenTex;
    int titleSound;

    //gui list
    List<GuiControl*> titleButtons;

private:
    

};