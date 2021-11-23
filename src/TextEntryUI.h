#ifndef CHATTER_FIRMWARE_TEXTENTRYUI_H
#define CHATTER_FIRMWARE_TEXTENTRYUI_H

#include <Arduino.h>
#include <Loop/LoopListener.h>
#include <Loop/LoopManager.h>
#include <Display/Sprite.h>

class TextEntry : public LoopListener {
public:
	TextEntry(uint8_t width, String text = " ");

	void activate();

	void loop(uint micros) override;

	const String& getText() const;

	void setText(const String& text);

private:
	static TextEntry* instance;

	uint8_t width = 0;
	String text;
	uint32_t cursorTime = 0;
	bool cursorBlink;
	Sprite* baseSprite;
};

#endif //CHATTER_FIRMWARE_TEXTENTRYUI_H
