#ifndef CHATTER_FIRMWARE_LISTITEM_H
#define CHATTER_FIRMWARE_LISTITEM_H

#include <Arduino.h>
#include <UI/CustomElement.h>

class ListItem : CustomElement {
public:
	ListItem(ElementContainer* parent,uint8_t icon, char text);

	void setSelected(uint8_t isSelected);

	void draw() override;

private:
	uint8_t icon;
	char text;
	uint8_t isSelected = false;
	static const char* const icons[5];
	Color* iconBuffer = nullptr;

	Sprite* baseSprite;
};

#endif //CHATTER_FIRMWARE_LISTITEM_H
