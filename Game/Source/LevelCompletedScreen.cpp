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
    completedSound = app->audio->LoadFx("Assets/Music/levelCompleted.wav");

    //unload the scene music
    app->scene->Disable();
    app->scene->active = false;
    app->entityManager->active = false;


    app->levelCompletedScreen->active = true;
    app->levelCompletedScreen->Enable();


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

        app->scene->GoToLevel2();

        app->scene->CameraLevel2();
    }

    return true;
}

bool LevelCompletedScreen::PostUpdate()
{
    app->render->DrawTexture(completedScreenTex, 0, 0);

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
