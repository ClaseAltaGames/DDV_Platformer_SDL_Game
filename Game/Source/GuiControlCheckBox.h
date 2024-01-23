#ifndef __GUICONTROLCHECKBOX_H__
#define __GUICONTROLCHECKBOX_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiControlCheckBox : public GuiControl
{

public:

	GuiControlCheckBox(uint32 id, SDL_Rect bounds, const char* text);
	virtual ~GuiControlCheckBox();

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

	bool isChecked;
	SString text;
};

#endif // __GUICONTROLCHECKBOX_H__