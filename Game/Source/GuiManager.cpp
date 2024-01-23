#include "GuiManager.h"
#include "App.h"
#include "Textures.h"

#include "GuiControlButton.h"
#include "GuiControlImage.h"
#include "GuiControlSlider.h"
#include "GuiControlCheckBox.h"
#include "Audio.h"

GuiManager::GuiManager(bool startEnabled) : Module(startEnabled)
{
	name.Create("guiManager");
}

GuiManager::~GuiManager() {}

bool GuiManager::Start()
{
	return true;
}

// L15: DONE1: Implement CreateGuiControl function that instantiates a new GUI control and add it to the list of controls
GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, Module* observer, SDL_Rect sliderBounds)
{
	GuiControl* guiControl = nullptr;

	//Call the constructor according to the GuiControlType

	switch (type)
	{
	case GuiControlType::BUTTON:
		guiControl = new GuiControlButton(id, bounds, text);
		break;
	case GuiControlType::TOGGLE:
		break;
	case GuiControlType::CHECKBOX:
		guiControl = new GuiControlCheckBox(id, bounds, text);
		break;
	case GuiControlType::SLIDER:
		guiControl = new GuiControlSlider(id, bounds, 0, 100, text);
		break;
	case GuiControlType::SLIDERBAR:
		break;
	case GuiControlType::COMBOBOX:
		break;
	case GuiControlType::DROPDOWNBOX:
		break;
	case GuiControlType::INPUTBOX:
		break;
	case GuiControlType::VALUEBOX:
		break;
	case GuiControlType::SPINNER:
		break;
	/*case GuiControlType::IMAGE:
		guiControl = new GuiControlImage(id, bounds, text);
		break;*/
	default:
		break;
	}
	//Set the observer
	guiControl->observer = observer;

	// Created GuiControls are add it to the list of controls
	guiControlsList.Add(guiControl);

	return guiControl;
}

bool GuiManager::Update(float dt)
{	
	
	return true;
}

bool GuiManager::PostUpdate()
{
	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr && control->data != nullptr)
	{
		control->data->PostUpdate();
		control = control->next;
	}
	return true;
}

bool GuiManager::PostLateUpdate()
{
	return true;
}

bool GuiManager::CleanUp()
{
	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		//ListItem<GuiControl*>* controlNext = control->next;
		RELEASE(control);
		//control = controlNext;
	}
	//guiControlsList.Clear();

	return true;

	
}

void GuiManager::DestroyGuiControl(GuiControl* toDestroy) {


	ListItem<GuiControl*>* control;
	for (control = guiControlsList.start; control != NULL; control = control->next)
	{
	if (control->data == toDestroy) {
			guiControlsList.Del(control);
			break;
		}
	}

}



