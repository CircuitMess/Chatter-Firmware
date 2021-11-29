#include "ListItem.h"
#include <SPIFFS.h>

const char* const ListItem::icons[] = {};

ListItem::ListItem(ElementContainer* parent, uint8_t icon, String text) : CustomElement(parent, 160, 20), icon(icon), text(text){
	iconBuffer = static_cast<Color*>(malloc(20*20*2));
	if(iconBuffer == nullptr){
		Serial.println("ListItem picture unpack error");
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
	baseSprite->setCursor(20 + (width/ 2 - baseSprite->textWidth(text))/ 2, 0);
	baseSprite->print(text);
}
