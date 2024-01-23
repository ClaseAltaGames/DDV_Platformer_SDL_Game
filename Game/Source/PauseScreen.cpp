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


    //app->audio->PlayMusic("Assets/Music/titleScreen.ogg", 1.0f);


    int wBt = 190;
    int hBt = 60;
    int posBtX = SCREEN_WIDTH / 2 - 95;
    int posBtY = SCREEN_HEIGHT / 2 + 230;

    //add music and fx slider
    sliderFx = app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 1021, "Fx", { posBtX, posBtY, wBt, hBt }, this);



    return true;
}

// Called each loop iteration
bool PauseScreen::Update(float dt)
{
    if (app->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
    {
       app->fadeToBlack->FadeToBlackTransition((Module*)app->pauseScreen, (Module*)app->scene, 0);
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

    return true;
}
