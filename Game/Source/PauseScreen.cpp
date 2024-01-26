#include "PauseScreen.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Input.h"
#include "Window.h"
#include "Audio.h"
#include "Scene.h"
#include "FadeToBlack.h"
#include "Map.h"
#include "EntityManager.h"
#include "Physics.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "GuiManager.h"
#include "LevelCompletedScreen.h"


#include "SDL/include/SDL.h"    

PauseScreen::PauseScreen(bool startEnabled) : Module(startEnabled)
{
}

// Destructor
PauseScreen::~PauseScreen()
{}

// Called before render is available
bool PauseScreen::Start()
{
    pauseScreenTex = app->tex->Load("Assets/Textures/pauseScreen.png");
    app->levelCompletedScreen->active = false;
    app->levelCompletedScreen->Disable();

    ListItem<GuiControl*>* controlListItem = nullptr;
    for (controlListItem = app->levelCompletedScreen->completedButtons.start; controlListItem != NULL; controlListItem = controlListItem->next)
    {
        app->guiManager->DestroyGuiControl(controlListItem->data);
    }
    app->levelCompletedScreen->completedButtons.Clear();

    return true;
}

// Called each loop iteration
bool PauseScreen::Update(float dt)
{    
    if (app->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
    {
        app->scene->pause = false;
        app->scene->active = true;
        app->scene->Enable();
        app->entityManager->active = true;
        app->entityManager->Enable();
        app->pauseScreen->active = false;
        app->pauseScreen->Disable();

        app->physics->DestroyCircle(app->scene->GetPlayerPbody());
        app->physics->CreateCircle(app->scene->GetPlayerPosition().x + 16, app->scene->GetPlayerPosition().y + 16, 8, bodyType::DYNAMIC);

        app->scene->GetPlayerPbody()->listener = app->scene->GetPlayer();
        app->scene->GetPlayerPbody()->ctype = ColliderType::PLAYER;

        app->fadeToBlack->FadeToBlackTransition((Module*)app->pauseScreen, (Module*)app->scene, 0);
    }

    app->scene->pause = true;
    //si pulsas esc en cualquier nivel aparece el menu de pausa
    if (app->scene->level1)
    {
        app->render->DrawTexture(pauseScreenTex, app->scene->GetPlayerPosition().x - 256, 0);
    }
    if (app->scene->level2)
    {
        app->render->DrawTexture(pauseScreenTex, app->scene->GetPlayerPosition().x - 256, 863);
    }
    if (app->scene->bossLevel)
    {
        app->render->DrawTexture(pauseScreenTex, app->scene->GetPlayerPosition().x - 256, 1613);
    }

    if (pauseButtons.At(2)->data->state == GuiControlState::PRESSED)
    {
        app->scene->pause = false;
        app->scene->active = true;
        app->scene->Enable();
        app->entityManager->active = true;
        app->entityManager->Enable();
        app->pauseScreen->active = false;
        app->pauseScreen->Disable();

        app->physics->DestroyCircle(app->scene->GetPlayerPbody());
        app->physics->CreateCircle(app->scene->GetPlayerPosition().x + 16, app->scene->GetPlayerPosition().y + 16, 8, bodyType::DYNAMIC);

        app->scene->GetPlayerPbody()->listener = app->scene->GetPlayer();
        app->scene->GetPlayerPbody()->ctype = ColliderType::PLAYER;

      
        app->fadeToBlack->FadeToBlackTransition((Module*)app->pauseScreen, (Module*)app->scene, 0);

	}

    if (pauseButtons.At(3)->data->state == GuiControlState::PRESSED)
    {
        if (app->win->fullscreen == false)
        {
            app->win->fullscreen = true;
            app->win->Fullscreen(app->win->fullscreen);
            pauseButtons.At(3)->data->state = GuiControlState::NORMAL;

            app->scene->GetPlayerPbody()->listener = app->scene->GetPlayer();
            app->scene->GetPlayerPbody()->ctype = ColliderType::PLAYER;
        }
        else
        {
            app->win->fullscreen = false;
            app->win->Fullscreen(app->win->fullscreen);
            pauseButtons.At(3)->data->state = GuiControlState::NORMAL;

            app->scene->GetPlayerPbody()->listener = app->scene->GetPlayer();
            app->scene->GetPlayerPbody()->ctype = ColliderType::PLAYER;
        }
    }

    if (pauseButtons.At(4)->data->state == GuiControlState::PRESSED)
    {
        //toggle vsync
      if (app->vsync == false)
        {
			app->vsync = true;
			pauseButtons.At(4)->data->state = GuiControlState::NORMAL;
		}
        else
        {
			app->vsync = false;
			pauseButtons.At(4)->data->state = GuiControlState::NORMAL;
		}
    }

    if (pauseButtons.At(5)->data->state == GuiControlState::PRESSED)
    {
        //go to title screen
        app->scene->pause = false;
        
        app->scene->active = false;
        app->scene->Disable();
        app->entityManager->active = false;
        app->entityManager->Disable();
        app->pauseScreen->active = false;
        app->pauseScreen->Disable();
        app->guiManager->active = false;
        app->guiManager->Disable();

        app->fadeToBlack->FadeToBlackTransition((Module*)app->pauseScreen, (Module*)app->titleScreen, 0);
    }

    if (pauseButtons.At(6)->data->state == GuiControlState::PRESSED)
    {
        SDL_Quit();
    }
    return true;
}

bool PauseScreen::PostUpdate()
{

    return true;
}

// Called before quitting
bool PauseScreen::CleanUp()
{
    if (pauseScreenTex != nullptr)
    {
        app->tex->UnLoad(pauseScreenTex);
        pauseScreenTex = nullptr;
    }
    //limpia la lista de pauseButtons
    

    return true;
}

void PauseScreen::CreatePauseButtons()
{
    if (this->isEnabled)
    {
        int wBt = 190;
        int hBt = 40;
        int posBtX = SCREEN_WIDTH / 2 - 95;
        int posBtY = SCREEN_HEIGHT / 2 + 140;
        pauseButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 1, "Fx", { posBtX, posBtY, wBt, hBt }, this));
        wBt = 190;
        hBt = 40;
        posBtX = SCREEN_WIDTH / 2 - 95;
        posBtY = SCREEN_HEIGHT / 2 + 70;
        pauseButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 2, "Music", { posBtX, posBtY, wBt, hBt }, this));
        //resume buton
        wBt = 190;
        hBt = 40;
        posBtX = SCREEN_WIDTH / 2 - 95;
        posBtY = SCREEN_HEIGHT / 2 - 60;
        pauseButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Resume", { posBtX, posBtY, wBt, hBt }, this));
        //fullscreen buton
        wBt = 30;
        hBt = 30;
        posBtX = SCREEN_WIDTH / 2 - 135;
        posBtY = SCREEN_HEIGHT / 2 + 250;
        pauseButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 4, "Fullscreen", { posBtX, posBtY, wBt, hBt }, this));
        //vsync buton
        wBt = 30;
        hBt = 30;
        posBtX = SCREEN_WIDTH / 2 + 15;
        posBtY = SCREEN_HEIGHT / 2 + 250;
        pauseButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 5, "VSync", { posBtX, posBtY, wBt, hBt }, this));
        //back to title and exits buttons
        wBt = 190;
        hBt = 40;
        posBtX = SCREEN_WIDTH / 2 - 95;
        posBtY = SCREEN_HEIGHT / 2;
        pauseButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, "Back to title", { posBtX, posBtY, wBt, hBt }, this));
        wBt = 190;
        hBt = 40;
        posBtX = SCREEN_WIDTH / 2 - 95;
        posBtY = SCREEN_HEIGHT / 2 + 200;
        pauseButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 7, "Exit", { posBtX, posBtY, wBt, hBt }, this));
    }
}