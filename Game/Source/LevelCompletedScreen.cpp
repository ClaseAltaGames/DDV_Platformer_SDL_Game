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

    //app->audio->PlayMusic("Assets/Music/titleScreen.ogg", 1.0f);

    //unload the scene music
    app->scene->Disable();
    app->scene->active = false;
    app->entityManager->active = false;
    
    app->guiManager->active = true;
    app->guiManager->Enable();
    app->levelCompletedScreen->active = true;
    app->levelCompletedScreen->Enable();

    //completedButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10, "Click Here To Next Level", {wBt, hBt, posBtX, posBtY}, this);

    return true;
}

// Called each loop iteration
bool LevelCompletedScreen::Update(float dt)
{
    if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
    {
        app->fadeToBlack->FadeToBlackTransition((Module*)app->levelCompletedScreen, (Module*)app->scene, 0);

        app->entityManager->active = true;

        app->scene->active = true;
        app->scene->Enable();

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
