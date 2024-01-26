#include "Credits.h"
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

#include "SDL/include/SDL.h"    

Credits::Credits(bool startEnabled) : Module(startEnabled)
{
}

// Destructor
Credits::~Credits()
{}

// Called before render is available
bool Credits::Start()
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

    ListItem<GuiControl*>* controlListTitle = nullptr;
    for (controlListTitle = app->titleScreen->titleButtons.start; controlListTitle != NULL; controlListTitle = controlListTitle->next)
    {
        app->guiManager->DestroyGuiControl(controlListTitle->data);
    }
    app->titleScreen->titleButtons.Clear();


    creditsTexture = app->tex->Load("Assets/Textures/credits.png");

    app->render->camera.x = 0;
    app->render->camera.y = 0;


    app->scene->Disable();
    app->scene->active = false;
    app->map->active = false;
    app->entityManager->active = false;
    app->physics->active = false;
    app->guiManager->active = true;
    app->guiManager->Enable();

    app->titleScreen->active = false;
    app->titleScreen->Disable();

    app->credits->active = true;
    app->credits->Enable();

    int wBt = 40;
    int hBt = 40;
    int posBtX = SCREEN_WIDTH / 2 - 480;
    int posBtY = SCREEN_HEIGHT / 2 - 330;
    creditsButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "<", { posBtX, posBtY, wBt, hBt }, this));

    return true;
}

// Called each loop iteration
bool Credits::Update(float dt)
{
    if (creditsButtons.At(0)->data->state == GuiControlState::PRESSED)
    {
        //go to title screen
        app->titleScreen->active = true;
        app->titleScreen->Enable();
        app->credits->active = false;
        app->credits->Disable();

        app->fadeToBlack->FadeToBlackTransition((Module*)app->credits, (Module*)app->titleScreen, 0);

    }
   
    return true;
}

bool Credits::PostUpdate()
{
    app->render->DrawTexture(creditsTexture, 0, 0);

    return true;
}

// Called before quitting
bool Credits::CleanUp()
{
    if (creditsTexture != nullptr)
    {
        app->tex->UnLoad(creditsTexture);
        creditsTexture = nullptr;
    }

    return true;
}

