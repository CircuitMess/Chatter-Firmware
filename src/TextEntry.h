#ifndef CHATTER_FIRMWARE_TEXTENTRY_H
#define CHATTER_FIRMWARE_TEXTENTRY_H

#include <Arduino.h>
#include <lvgl.h>
#include "LVObject.h"
#include <string>
#include <Input/InputListener.h>
#include <Loop/LoopListener.h>

class TextEntry : public LVObject, private InputListener, public LoopListener {
public:
	TextEntry(lv_obj_t* parent, const std::string& text = "", uint32_t maxLength = -1);
	void setTextColor(lv_color_t color);
	void setPlaceholder(const std::string& text);
	void setText(const std::string& text);
	const std::string& getText() const;

	void keyPress(uint8_t i);
	void clear();

	void start();
	void stop();
	bool isActive() const;

	void loop(uint micros) override;

private:
	void buttonPressed(uint i) override;

	static const char* characters[];
	static const std::map<uint8_t, uint8_t> keyMap;

	std::string text;

	lv_style_t entryFocus;
	lv_obj_t* entry;
	bool active = false;

	int8_t currentKey = -1; // currently active key
	uint8_t index = 0; // character under the key
	uint32_t keyTime = 0; // when the key was last pressed
};

#endif //CHATTER_FIRMWARE_TEXTENTRY_H
