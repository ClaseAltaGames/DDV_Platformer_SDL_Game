#include "TitleScreen.h"
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
#include "PauseScreen.h"
#include "LevelCompletedScreen.h"
#include "GuiManager.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "Credits.h"


#include "SDL/include/SDL.h"    

TitleScreen::TitleScreen(bool startEnabled) : Module(startEnabled)
{
}

// Destructor
TitleScreen::~TitleScreen()
{}

// Called before render is available
bool TitleScreen::Start()
{

    ListItem<GuiControl*>* controlListItem = nullptr;
    for (controlListItem = app->levelCompletedScreen->completedButtons.start; controlListItem != NULL; controlListItem = controlListItem->next)
    {
        app->guiManager->DestroyGuiControl(controlListItem->data);
    }
    app->levelCompletedScreen->completedButtons.Clear();

    ListItem<GuiControl*>* controlListPause = nullptr;
    for (controlListPause = app->pauseScreen->pauseButtons.start; controlListPause != NULL; controlListPause = controlListPause->next)
    {
        app->guiManager->DestroyGuiControl(controlListPause->data);
    }
    app->pauseScreen->pauseButtons.Clear();

    ListItem<GuiControl*>* controlListCredits = nullptr;
    for (controlListCredits = app->credits->creditsButtons.start; controlListCredits != NULL; controlListCredits = controlListCredits->next)
    {
        app->guiManager->DestroyGuiControl(controlListCredits->data);
    }
    app->credits->creditsButtons.Clear();

    
    
    titleScreenTex = app->tex->Load("Assets/Textures/titleScreen.png");

    app->render->camera.x = 0;
    app->render->camera.y = 0;

    //app->audio->PlayMusic("Assets/Music/titleScreen.ogg", 1.0f);
    titleSound = app->audio->LoadFx("Assets/Music/onlymp3.to-Bully-Soundtrack-Main-Theme-7RU7CohvsMU-192k-1704300284.wav");
    

    app->scene->Disable();
    app->scene->active = false;
    app->map->active = false;
    app->entityManager->active = false;
	app->physics->active = false;
    app->guiManager->active = true;
    app->guiManager->Enable();

    app->credits->active = false;
    app->credits->Disable();

    app->titleScreen->active = true;
    app->titleScreen->Enable();

    CreateTitleButtons();

    return true;
}

// Called each loop iteration
bool TitleScreen::Update(float dt)
{
    if (titleButtons.At(0)->data->state == GuiControlState::PRESSED)
    {
        //set pboody to initial position
        app->physics->DestroyCircle(app->scene->GetPlayerPbody());
        app->physics->CreateCircle(3 + 16, 215 + 16, 8, bodyType::DYNAMIC);
        app->scene->GetPlayerPbody()->listener = app->scene->GetPlayer();
        app->scene->GetPlayerPbody()->ctype = ColliderType::PLAYER;
       
        app->audio->UnloadFx(titleSound);
        app->fadeToBlack->FadeToBlackTransition((Module*)app->titleScreen, (Module*)app->scene, 0);

        app->titleScreen->active = false;
        app->titleScreen->Disable();

        app->scene->active = true;
        app->scene->Enable();
        app->map->active = true;
        app->entityManager->active = true;
        app->physics->active = true;

        //plays the scene music
        app->audio->PlayMusic("Assets/Music/Song1.ogg", 1.0f);
	}  
    if (titleButtons.At(1)->data->state == GuiControlState::PRESSED)
    {
        app->audio->UnloadFx(titleSound);
        app->fadeToBlack->FadeToBlackTransition((Module*)app->titleScreen, (Module*)app->scene, 0);

        app->titleScreen->active = false;
        app->titleScreen->Disable();

        app->scene->active = true;
        app->scene->Enable();
        app->map->active = true;
        app->entityManager->active = true;
        app->physics->active = true;

        if (app->scene->level2)
        {
            app->scene->CameraLevel2();
        }
        if (app->scene->bossLevel)
        {
            app->scene->CameraBoss();
        }

        //plays the scene music
        app->audio->PlayMusic("Assets/Music/Song1.ogg", 1.0f);
    } 

    if (titleButtons.At(2)->data->state == GuiControlState::PRESSED)
    {
        //go to credits
        app->fadeToBlack->FadeToBlackTransition((Module*)app->titleScreen, (Module*)app->credits, 0);
        app->titleScreen->Disable();
        app->titleScreen->active = false;
        app->credits->active = true;
        app->credits->Enable();

    }
    
    if (titleButtons.At(3) != nullptr && titleButtons.At(3)->data->state == GuiControlState::PRESSED)
    {
        SDL_Quit();
    }
    return true;
}

bool TitleScreen::PostUpdate()
{
    app->render->DrawTexture(titleScreenTex, 0, 0);

    return true;
}

// Called before quitting
bool TitleScreen::CleanUp()
{
    if (titleScreenTex != nullptr)
    {
        app->tex->UnLoad(titleScreenTex);
        titleScreenTex = nullptr;
    }

    return true;
}

void TitleScreen::CreateTitleButtons()
{
    if (this->isEnabled)
    {
        int wBt = 190;
        int hBt = 40;
        int posBtX = SCREEN_WIDTH / 2 - 220;
        int posBtY = SCREEN_HEIGHT / 2 - 10;
        titleButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Play", { posBtX, posBtY, wBt, hBt }, this));
        wBt = 190;
        hBt = 40;
        posBtX = SCREEN_WIDTH / 2 - 220;
        posBtY = SCREEN_HEIGHT / 2 + 100;
        titleButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Continue", { posBtX, posBtY, wBt, hBt }, this));
        //resume buton
        wBt = 190;
        hBt = 40;
        posBtX = SCREEN_WIDTH / 2 - 220;
        posBtY = SCREEN_HEIGHT / 2 + 210;
        titleButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Credits", { posBtX, posBtY, wBt, hBt }, this));
        //fullscreen buton
        wBt = 30;
        hBt = 30;
        posBtX = SCREEN_WIDTH / 2 + 450;
        posBtY = SCREEN_HEIGHT / 2 - 330;
        titleButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "X", { posBtX, posBtY, wBt, hBt }, this));
    }
}