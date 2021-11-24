#include "TextEntry.h"
#include <Input/InputShift.h>
#include <Loop/LoopManager.h>

TextEntry* TextEntry::instance = nullptr;

TextEntry::TextEntry(uint8_t width, String text) : width(width){

	instance = this;

}

void TextEntry::activate(){
	Input::getInstance()->addListener(this);
	LoopManager::addListener(this);
	if(cursorBlink){
		baseSprite->setTextColor(TFT_BLACK);
		baseSprite->setTextFont(1);
		baseSprite->setCursor(width + 1,1);
		baseSprite->print('|');
	}
}

const String& TextEntry::getText() const{
	return text;
}

void TextEntry::setText(const String& text){
	TextEntry::text = text;
}

void TextEntry::loop(uint micros){
	baseSprite->clear(TFT_BLACK);
	cursorTime += millis();
	if(cursorTime >= 350000){
		cursorTime = 0;
		cursorBlink = !cursorBlink;
	}
}
