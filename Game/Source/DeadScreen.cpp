#include "DeadScreen.h"
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

DeadScreen::DeadScreen(bool startEnabled) : Module(startEnabled)
{
}

// Destructor
DeadScreen::~DeadScreen()
{}

// Called before render is available
bool DeadScreen::Start()
{
    deadScreenTex = app->tex->Load("Assets/Textures/deadScreen.png");

    app->render->camera.x = 0;
    app->render->camera.y = 0;

    //unload the scene music
    app->scene->Disable();
    app->scene->active = false;
    app->map->active = false;
    app->entityManager->active = false;
    app->physics->active = false;



    return true;
}

// Called each loop iteration
bool DeadScreen::Update(float dt)
{
    //si pulsas space fade to black a la escena
    if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
    {
        app->fadeToBlack->FadeToBlackTransition((Module*)app->deadScreen, (Module*)app->scene, 0);
        if (app->scene->level1)
        {
            app->physics->DestroyCircle(app->scene->GetPlayerPbody());
            app->physics->CreateCircle(3 + 16, 215 + 16, 8, bodyType::DYNAMIC);
            app->scene->CameraLevel1();
        }
        if (app->scene->level2)
        {
            app->physics->DestroyCircle(app->scene->GetPlayerPbody());
            app->physics->CreateCircle(3 + 16, 215 + 16, 8, bodyType::DYNAMIC);
            app->scene->CameraLevel2();
        }
        if (app->scene->bossLevel)
        {
            app->physics->DestroyCircle(app->scene->GetPlayerPbody());
            app->physics->CreateCircle(3 + 16, 215 + 16, 8, bodyType::DYNAMIC);
            app->scene->CameraBoss();            
        }
        if (app->scene->DetectCheckpoint())
        {
            app->physics->DestroyCircle(app->scene->GetPlayerPbody());
            app->physics->CreateCircle(2425 + 16, 239 + 16, 8, bodyType::DYNAMIC);
        }

        app->scene->GetPlayerPbody()->listener = app->scene->GetPlayer();
        app->scene->GetPlayerPbody()->ctype = ColliderType::PLAYER;

        //play scene music
        app->audio->PlayMusic("Assets/Music/Song1.ogg", 1.0f);

        app->scene->Enable();
        app->scene->active = true;
        app->map->active = true;
        app->entityManager->active = true;
        app->physics->active = true;

        app->deadScreen->active = false;
        app->deadScreen->Disable();
        
    }
    return true;
}

bool DeadScreen::PostUpdate()
{
    app->render->DrawTexture(deadScreenTex, 0, 0);
   
    return true;
}

// Called before quitting
bool DeadScreen::CleanUp()
{
    if (deadScreenTex != nullptr)
    {
        app->tex->UnLoad(deadScreenTex);
        deadScreenTex = nullptr;
    }

    return true;
}