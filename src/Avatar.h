#ifndef CHATTER_FIRMWARE_AVATAR_H
#define CHATTER_FIRMWARE_AVATAR_H

#include <Arduino.h>
#include <UI/ElementContainer.h>
#include <UI/CustomElement.h>

class Avatar : public CustomElement{
public:
	Avatar(ElementContainer* parent, uint8_t avatarIndex);

	virtual ~Avatar();

	void draw() override;

	uint8_t avatarIndex = 0;

private:
	String pictures[20];
	Color* avatarBuffer = nullptr;
	Sprite* baseSprite;
};

#endif //CHATTER_FIRMWARE_AVATAR_H
