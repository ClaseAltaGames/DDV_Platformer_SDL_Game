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

    void CreatePauseButtons();

    bool CleanUp();

public:

    SDL_Texture* pauseScreenTex;

    GuiControlSlider* sliderFx;
    GuiControlSlider* sliderMusic;

    GuiControlCheckBox* checkBoxFullscreen;
    GuiControlCheckBox* checkBoxVSync;

    bool completedFxAvailable = false;
    float volume = 0;

    bool showPause = false;
    List<GuiControl*> pauseButtons;

private:
    GuiControlButton* gcButtom;
    GuiControl* gcCloseCredits;

    bool showSettings = false;
    bool _showSettings = false;

    bool showCredits = false;
    bool _showCredits = false;
    


};