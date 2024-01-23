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

    app->guiManager->DestroyGuiControl(app->pauseScreen->sliderMusic);
    app->guiManager->DestroyGuiControl(app->pauseScreen->sliderFx);

    app->render->camera.x = 0;
    app->render->camera.y = 0;

    //app->audio->PlayMusic("Assets/Music/titleScreen.ogg", 1.0f);


    int wBt = 190;
    int hBt = 60;
    int posBtX = SCREEN_WIDTH / 2 - 95;
    int posBtY = SCREEN_HEIGHT / 2 + 230;

    //SDL_Rect btPos = { SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 + 240, 190,60 };

    completedButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10, "Next level", {posBtX, posBtY, wBt, hBt}, this);

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
        //plays the scene music

        app->audio->PlayMusic("Assets/Music/Song1.ogg", 1.0f);

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
