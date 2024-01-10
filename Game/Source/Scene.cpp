#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Physics.h"
#include "Player.h"
#include "WEnemies.h"
#include "EntityManager.h"
#include "TitleScreen.h"
#include "LevelCompletedScreen.h"
#include "DeadScreen.h"
#include "FadeToBlack.h"
#include "GuiManager.h"
#include "Health.h"
#include "Jabon.h"


#include "Defs.h"
#include "Log.h"

Scene::Scene(bool startEnabled) : Module(startEnabled)
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	
	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access

	if (config.child("player")) {
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = config.child("player");

		/*if (config.child("wenemy")) {
				wenemy = (WEnemies*)app->entityManager->CreateEntity(EntityType::WENEMIES);
				wenemy->parameters = config.child("wenemy");
		}*/
		for (pugi::xml_node wenemyNode = config.child("wenemy"); wenemyNode; wenemyNode = wenemyNode.next_sibling("wenemy"))
		{
			WEnemies* wenemy = (WEnemies*)app->entityManager->CreateEntity(EntityType::WENEMIES);
			wenemy->parameters = wenemyNode;
			
			wenemyList.Add(wenemy);
		}
		for (pugi::xml_node fenemyNode = config.child("fenemy"); fenemyNode; fenemyNode = fenemyNode.next_sibling("fenemy"))
		{
			FEnemies* fenemy = (FEnemies*)app->entityManager->CreateEntity(EntityType::FENEMIES);
			fenemy->parameters = fenemyNode;
			
			fenemyList.Add(fenemy);
		}
		for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
		{
			Jabon* item = (Jabon*)app->entityManager->CreateEntity(EntityType::JABON);
			item->parameters = itemNode;
		}
		for (pugi::xml_node potiNode = config.child("poti"); potiNode; potiNode = potiNode.next_sibling("poti"))
		{
			Poti* poti = (Poti*)app->entityManager->CreateEntity(EntityType::HEALTH);
			poti->parameters = potiNode;
		}
		if (config.child("map")) {
			//Get the map name from the config file and assigns the value in the module
			app->map->name = config.child("map").attribute("name").as_string();
			app->map->path = config.child("map").attribute("path").as_string();
		}

		return ret;
	}
}
// Called before the first frame
bool Scene::Start()
{
	app->titleScreen->active = false;
	app->titleScreen->Disable();

	app->deadScreen->active = false;
	app->deadScreen->Disable();

	app->levelCompletedScreen->active = false;
	app->levelCompletedScreen->Disable();

	app->guiManager->active = false;
	app->guiManager->Disable();



	app->tex->UnLoad(app->titleScreen->titleScreenTex);

	fondo = app->tex->Load("Assets/Textures/fondo.png");
	//app->audio->PlayMusic("Assets/Music/Song1.ogg", 1.0f);
	// NOTE: We have to avoid the use of paths in the code, we will move it later to a config file
	//img = app->tex->Load("Assets/Textures/test.png");
	
	//Music is commented so that you can add your own music
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(img, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());

	// Texture to highligh mouse position 
	mouseTileTex = app->tex->Load("Assets/Maps/tileSelection.png");

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	
	app->levelCompletedScreen->active = false;
	app->levelCompletedScreen->Disable();
	app->render->DrawTexture(fondo, player->position.x - 340, 0);

	float camSpeed = 1; 

	if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y -= (int)ceil(camSpeed * dt);

	if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y += (int)ceil(camSpeed * dt);

	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x -= (int)ceil(camSpeed * dt);

	if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x += (int)ceil(camSpeed * dt);

	//iPoint mousePos;
	//app->input->GetMousePosition(mousePos.x, mousePos.y);
	//iPoint mouseTile = app->map->WorldToMap(mousePos.x - app->render->camera.x,
	//	mousePos.y - app->render->camera.y);

	//// Render a texture where the mouse is over to highlight the tile, use the texture 'mouseTileTex'
	//iPoint highlightedTileWorld = app->map->MapToWorld(mouseTile.x, mouseTile.y);
	//app->render->DrawTexture(mouseTileTex, highlightedTileWorld.x, highlightedTileWorld.y);

	//iPoint origin = iPoint(2, 2);

	////If mouse button is pressed modify player position
	//if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
	//	player->position = iPoint(highlightedTileWorld.x, highlightedTileWorld.y);
	//	app->map->pathfinding->CreatePath(origin, mouseTile);
	//}

	//// L13: Get the latest calculated path and draw
	//const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
	//for (uint i = 0; i < path->Count(); ++i)
	//{
	//	iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
	//	app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
	//}


	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();


	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool Scene::LoadState(pugi::xml_node node) {

	b2Vec2 posMetres = b2Vec2(PIXEL_TO_METERS(node.child("player").attribute("x").as_int()), 
							  PIXEL_TO_METERS(node.child("player").attribute("y").as_int()));

	player->pbody->body->SetTransform(posMetres,0);

	

	//load the position of the wenemyList in the xml file detecting all of enemies and sibling
	for (pugi::xml_node wenemyNode = node.child("wenemy"); wenemyNode; wenemyNode = wenemyNode.next_sibling("wenemy"))
	{
		b2Vec2 posMetres = b2Vec2(PIXEL_TO_METERS(wenemyNode.attribute("x").as_int()),
						PIXEL_TO_METERS(wenemyNode.attribute("y").as_int()));

		int id = wenemyNode.attribute("id").as_int();

		wenemyList.At(id)->data->ebody->body->SetTransform(posMetres, 0);
	}
	
	//load the position of the fenemyList in the xml file detecting all of enemies and sibling
	for (pugi::xml_node fenemyNode = node.child("fenemy"); fenemyNode; fenemyNode = fenemyNode.next_sibling("fenemy"))
	{
		b2Vec2 posMetres = b2Vec2(PIXEL_TO_METERS(fenemyNode.attribute("x").as_int()),
						PIXEL_TO_METERS(fenemyNode.attribute("y").as_int()));

		int id = fenemyNode.attribute("id").as_int();

		fenemyList.At(id)->data->ebody->body->SetTransform(posMetres, 0);
	}
		
	return true;
}

bool Scene::SaveState(pugi::xml_node node) {
	
	pugi::xml_node playerNode = node.append_child("player");
	playerNode.append_attribute("x").set_value(player->position.x);
	playerNode.append_attribute("y").set_value(player->position.y);
	

	// save the position of the wenemy with the list in the xml file detecting the id of the wenemy
	for (int i = 0; i < 13; i++)
	{
		pugi::xml_node wenemyNode = node.append_child("wenemy");
		wenemyNode.append_attribute("x").set_value(wenemyList.At(i)->data->position.x);
		wenemyNode.append_attribute("y").set_value(wenemyList.At(i)->data->position.y);

		wenemyNode.append_attribute("id").set_value(i);
	}

	// save the position of the fenemy with the list in the xml file 
	for (int i = 0; i < 7; i++)
	{
		pugi::xml_node fenemyNode = node.append_child("fenemy");
		fenemyNode.append_attribute("x").set_value(fenemyList.At(i)->data->position.x);
		fenemyNode.append_attribute("y").set_value(fenemyList.At(i)->data->position.y);

		fenemyNode.append_attribute("id").set_value(i);
	}


	return true;
}