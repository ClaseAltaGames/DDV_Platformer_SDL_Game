#ifndef __GUICONTROLSLIDER_H__
#define __GUICONTROLSLIDER_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiControlSlider : public GuiControl
{

public:

	GuiControlSlider(uint32 id, SDL_Rect bounds, int minValue, int maxValue, const char* text);
	virtual ~GuiControlSlider();

	// Called each loop iteration
	bool Update(float dt);
	bool PostUpdate();
	void Draw();

private:


	int minValue, maxValue, value;

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;
};

#endif // __GUICONTROLSLIDER_H__