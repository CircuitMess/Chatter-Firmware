#ifndef CHATTER_FIRMWARE_EDITABLEAVATAR_H
#define CHATTER_FIRMWARE_EDITABLEAVATAR_H

#include <Arduino.h>
#include <UI/CustomElement.h>
#include "Avatar.h"
#include <Loop/LoopListener.h>
#include <Input/InputShift.h>

class EditableAvatar : public Avatar, public LoopListener, private InputListener{
public:
	EditableAvatar(ElementContainer* parent, uint8_t avatarIndex);

	void draw() override;

	void loop(uint micros) override;

	void setSelected(bool selected);

	void setActive(bool active);

	uint8_t getAvatarIndex() const;

private:
	bool isSelected = false;
	bool active = false;
	uint8_t avatarIndex;
	bool blinkState;
	uint blinkMicros;

	void buttonPressed(uint id) override;

	Sprite* baseSprite;
};
#endif //CHATTER_FIRMWARE_EDITABLEAVATAR_H
