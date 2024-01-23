//#include "GuiControlImage.h"
//#include "Render.h"
//#include "App.h"
//#include "Audio.h"
//#include "Log.h"
//#include "Window.h"
//#include "Textures.h"
//
//GuiControlImage::GuiControlImage(uint32 id, SDL_Rect bounds, const char* path) : GuiControl(GuiControlType::BUTTON, id)
//{
//	this->bounds = bounds;
//	this->path = path;
//
//	canClick = false;
//	drawBasic = false;
//
//	texture = app->tex->Load("Output\Assets\UI\TittleScreen");
//}
//
//GuiControlImage::~GuiControlImage()
//{
//
//}
//
//bool GuiControlImage::Update(float dt)
//{
//	
//
//	return false;
//}
//
//bool GuiControlImage::PostUpdate()
//{
//	if (state != GuiControlState::DISABLED)
//	{
//		// L15: DONE 3: Update the state of the GUiButton according to the mouse position
//		app->input->GetMousePosition(mouseX, mouseY);
//		
//		mouseX *= app->win->GetScale();
//		mouseY *= app->win->GetScale();
//
//		//If the position of the mouse if inside the bounds of the button 
//		if (mouseX > bounds.x && mouseX < bounds.x + bounds.w && mouseY > bounds.y && mouseY < bounds.y + bounds.h) {
//
//			state = GuiControlState::FOCUSED;
//
//			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
//				state = GuiControlState::PRESSED;
//			}
//
//			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
//				NotifyObserver();
//			}
//		}
//		else {
//			state = GuiControlState::NORMAL;
//		}
//
//		//L15: DONE 4: Draw the button according the GuiControl State
//		/*switch (state)
//		{
//		case GuiControlState::DISABLED:
//			app->render->DrawRectangle(bounds, 200, 200, 200, 255, true, false);
//			break;
//		case GuiControlState::NORMAL:
//			app->render->DrawRectangle(bounds, 0, 0, 255, 255, true, false);
//			break;
//		case GuiControlState::FOCUSED:
//			app->render->DrawRectangle(bounds, 0, 0, 20, 255, true, false);
//			break;
//		case GuiControlState::PRESSED:
//			app->render->DrawRectangle(bounds, 0, 255, 0, 255, true, false);
//			break;
//		}*/
//
//		app->render->DrawText(text.GetString(), bounds.x, bounds.y, bounds.w, bounds.h);
//
//		app->render->DrawTexture(texture, bounds.x, bounds.y, 1.0f);
//
//	}
//	return false;
//}
//
