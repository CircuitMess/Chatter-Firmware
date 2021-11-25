#include "TextEntry.h"
#include <Input/InputShift.h>
#include <Loop/LoopManager.h>
#include <Chatter.h>

TextEntry* TextEntry::instance = nullptr;

TextEntry::TextEntry(ElementContainer* parent, uint height, uint8_t width, String text = " ") : CustomElement(parent, width, height), width(width),height(height){

	instance = this;

}

void TextEntry::activate(){
	Input::getInstance()->addListener(this);
	LoopManager::addListener(this);
	if(cursorBlink){
		baseSprite->setTextColor(TFT_BLACK);
		baseSprite->setTextFont(1);
		baseSprite->setCursor(baseSprite->textWidth(text)+1 ,height);
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

void TextEntry::buttonPressed(uint id){
	if(id == BTN_BACK){
		Input::getInstance()->removeListener(this);
		LoopManager::removeListener(this);
	}else if(id == BTN_1){
		baseSprite->setTextColor(TFT_BLACK);
		baseSprite->setTextFont(1);
		baseSprite->setCursor(baseSprite->textWidth(text) ,height);
		baseSprite->print('1');

	}
}
