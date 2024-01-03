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
#include "IntroScreen.h"

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
    titleScreenTex = app->tex->Load("Assets/Textures/titleScreen.png");

    app->render->camera.x = 0;
    app->render->camera.y = 0;

    app->audio->PlayMusic("Assets/Music/titleScreen.ogg", 1.0f);

    app->scene->Disable();
    app->scene->active = false;
    app->map->active = false;
    app->entityManager->active = false;
	app->physics->active = false;

    app->titleScreen->active = true;
    app->titleScreen->Enable();

    return true;
}

// Called each loop iteration
bool TitleScreen::Update(float dt)
{
    if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
    {
        app->fadeToBlack->FadeToBlackTransition((Module*)app->titleScreen, (Module*)app->scene, 0);

        app->titleScreen->active = false;
        app->titleScreen->Disable();

        app->scene->active = true;
        app->scene->Enable();
        app->map->active = true;
        app->entityManager->active = true;
        app->physics->active = true;
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
