#include "ListItem.h"
#include <SPIFFS.h>

ListItem::ListItem(ElementContainer* parent, uint8_t icon, char text) : CustomElement(parent, 20, 20), icon(icon), text(text){
	iconBuffer = static_cast<Color*>(malloc(20*20*2));
	if(iconBuffer == nullptr){
		Serial.println("Avatar picture unpack error");
		return;
	}

	fs::File iconFile = SPIFFS.open(icons[icon]);
	iconFile.read(reinterpret_cast<uint8_t*>(iconBuffer), 20 * 20 * 2);
	iconFile.close();
}

void ListItem::setSelected(uint8_t isSelected){
	ListItem::isSelected = isSelected;
}

void ListItem::draw(){
	baseSprite->clear(TFT_BLACK);
	baseSprite->drawIcon(iconBuffer,0,0,20,20,1,TFT_BLACK);
	baseSprite->setTextColor(TFT_BLACK);
	baseSprite->setTextFont(1);
	baseSprite->setTextSize(1);
	baseSprite->setCursor(20, 0);
	baseSprite->print(text);
}
