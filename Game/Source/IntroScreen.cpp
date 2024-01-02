#include "IntroScreen.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Input.h"
#include "Window.h"
#include "Audio.h"
#include "Scene.h"
#include "FadeToBlack.h"

IntroScreen::IntroScreen(bool startEnabled) : Module(startEnabled)
{
    /*for (int fila = 0; fila < 24; fila++)
    {
        for (int columna = 0; columna < 5; columna++)
        {
            int frameX = columna * SCREEN_WIDTH;
            int frameY = fila * SCREEN_HEIGHT;
            introScreenAnim.PushBack({ frameX, frameY, SCREEN_WIDTH, SCREEN_HEIGHT });
        }
    }

    introScreenAnim.speed = 0.02f;
    introScreenAnim.loop = false;*/
  
}

// Destructor
IntroScreen::~IntroScreen()
{}

// Called before render is available
bool IntroScreen::Start()
{
    /*app->scene->Disable();
    introScreenTex = app->tex->Load("Assets/Textures/introScreen1.png");*/

    return true;
}

// Called each loop iteration
bool IntroScreen::Update(float dt)
{
   /* introScreenAnim.Update();
    if (app->input->GetKey(SDL_SCANCODE_SPACE))
    {
		app->fadeToBlack->FadeToBlackTransition(this, (Module*)app->scene, 90);
	}*/
    return true;
}

bool IntroScreen::PostUpdate()
{
   /* app->render->DrawTexture(introScreenTex, 0, 0, &introScreenAnim.GetCurrentFrame());*/

	return true;
}

// Called before quitting
bool IntroScreen::CleanUp()
{
    return true;
}
