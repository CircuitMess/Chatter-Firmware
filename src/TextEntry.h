#ifndef CHATTER_FIRMWARE_TEXTENTRY_H
#define CHATTER_FIRMWARE_TEXTENTRY_H

#include <Arduino.h>
#include <Loop/LoopListener.h>
#include <Loop/LoopManager.h>
#include <Display/Sprite.h>
#include <Input/InputListener.h>

class TextEntry : public LoopListener, private InputListener{
public:
	TextEntry(uint8_t width, String text);

	void activate();

	void loop(uint micros) override;

	const String& getText() const;

	void setText(const String& text);

private:
	static TextEntry* instance;

	void buttonPressed(uint id) override;

	uint8_t width = 0;
	String text;
	uint32_t cursorTime = 0;
	bool cursorBlink;
	Sprite* baseSprite;
};

#endif //CHATTER_FIRMWARE_TEXTENTRY_H
