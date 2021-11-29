#include "Avatar.h"
#include <SPIFFS.h>

Avatar::Avatar( ElementContainer* parent, uint8_t avatarIndex) : CustomElement(parent, 20, 20),avatarIndex(avatarIndex){
	avatarBuffer = static_cast<Color*>(malloc(20*20*2));
	if(avatarBuffer == nullptr){
		Serial.println("Avatar picture unpack error");
		return;
	}

	fs::File avFile = SPIFFS.open(pictures[avatarIndex]);
	avFile.read(reinterpret_cast<uint8_t*>(avatarBuffer), 20 * 20 * 2);
	avFile.close();
}

Avatar::~Avatar(){
	free(avatarBuffer);
}

void Avatar::draw(){
	baseSprite->clear(TFT_BLACK);
	baseSprite->drawIcon(avatarBuffer,getTotalX(),getTotalY(),20,20,1,TFT_BLACK);
}
