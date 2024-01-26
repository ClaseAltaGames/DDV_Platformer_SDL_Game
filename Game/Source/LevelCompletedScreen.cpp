#include "LevelCompletedScreen.h"
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
#include "PauseScreen.h"
#include "TitleScreen.h"
#include "Credits.h"


#include "SDL/include/SDL.h"    

LevelCompletedScreen::LevelCompletedScreen(bool startEnabled) : Module(startEnabled)
{
}

// Destructor
LevelCompletedScreen::~LevelCompletedScreen()
{}

// Called before render is available
bool LevelCompletedScreen::Start()
{
    completedScreenTex = app->tex->Load("Assets/Textures/levelCompleteScreen.png");

    app->render->camera.x = 0;
    app->render->camera.y = 0;

    app->pauseScreen->Disable();
    app->pauseScreen->active = false;

    ListItem<GuiControl*>* controlListItem = nullptr;
    for (controlListItem = app->pauseScreen->pauseButtons.start; controlListItem != NULL; controlListItem = controlListItem->next)
    {
        app->guiManager->DestroyGuiControl(controlListItem->data);
    }
    app->pauseScreen->pauseButtons.Clear();

    ListItem<GuiControl*>* controlListTitle = nullptr;
    for (controlListTitle = app->titleScreen->titleButtons.start; controlListTitle != NULL; controlListTitle = controlListTitle->next)
    {
        app->guiManager->DestroyGuiControl(controlListTitle->data);
    }
    app->titleScreen->titleButtons.Clear();

    ListItem<GuiControl*>* controlListCredits = nullptr;
    for (controlListCredits = app->credits->creditsButtons.start; controlListCredits != NULL; controlListCredits = controlListCredits->next)
    {
        app->guiManager->DestroyGuiControl(controlListCredits->data);
    }
    app->credits->creditsButtons.Clear();




    int wBt = 190;
    int hBt = 60;
    int posBtX = SCREEN_WIDTH / 2 - 95;
    int posBtY = SCREEN_HEIGHT / 2 + 230;

   
    completedButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10, "Next level", {posBtX, posBtY, wBt, hBt}, this);

    completedButtons.Add(completedButton);

    return true;
}

// Called each loop iteration
bool LevelCompletedScreen::Update(float dt)
{    
    if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || completedButton->state == GuiControlState::PRESSED)
    {
        app->scene->CheckpointOFF();
        app->fadeToBlack->FadeToBlackTransition((Module*)app->levelCompletedScreen, (Module*)app->scene, 0);

        app->entityManager->active = true;

        app->scene->active = true;
        app->scene->Enable();

        app->guiManager->active = true;
        app->guiManager->Enable();      

        if (app->scene->level1)
        {
           app->scene->GoToLevel2();
           app->scene->CameraLevel2();
        }
        if (app->scene->level2)
        {
            app->scene->GoToBoss();
            app->scene->CameraBoss();
        }
    }

    app->render->DrawTexture(completedScreenTex, 0, 0);

    return true;
}

bool LevelCompletedScreen::PostUpdate()
{


    return true;
}

// Called before quitting
bool LevelCompletedScreen::CleanUp()
{
    if (completedScreenTex != nullptr)
    {
        app->tex->UnLoad(completedScreenTex);
        completedScreenTex = nullptr;
    }

    return true;
}
