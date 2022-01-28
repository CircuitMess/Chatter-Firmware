#ifndef CHATTER_FIRMWARE_SETTINGSSCREEN_H
#define CHATTER_FIRMWARE_SETTINGSSCREEN_H

#include "../LVScreen.h"

class SettingsScreen : public LVScreen {
public:
	SettingsScreen();

private:
	lv_obj_t* sound;
	lv_obj_t* sleepTime;
	lv_obj_t* screenBrightness;
	lv_obj_t* factoryReset;
	lv_obj_t* HWTest;
	lv_obj_t* version;
};

#endif //CHATTER_FIRMWARE_SETTINGSSCREEN_H
