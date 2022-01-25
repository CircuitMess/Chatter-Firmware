#ifndef CHATTER_FIRMWARE_PROFILESCREEN_H
#define CHATTER_FIRMWARE_PROFILESCREEN_H

#include <Arduino.h>
#include <lvgl.h>
#include "LVScreen.h"

class ProfileScreen : public LVScreen{
public:
	ProfileScreen();
protected:
	lv_style_t profile;
	lv_style_t styleFocus;
	lv_style_t nameStyleFocus;
	lv_style_t styleFocus2;
	lv_style_t styleDef;
	lv_style_t nameStyleDef;
	lv_style_t styleDef2;
	lv_style_t screenStyle;
	lv_obj_t* name;

	void exit();
};

#endif //CHATTER_FIRMWARE_PROFILESCREEN_H