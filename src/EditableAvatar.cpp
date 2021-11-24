#include "EditableAvatar.h"

EditableAvatar::EditableAvatar(ElementContainer* parent, uint8_t avatarIndex) : Avatar(parent, avatarIndex), avatarIndex(avatarIndex){

}

void EditableAvatar::draw(){
	Avatar::draw();
	if(isSelected){
		baseSprite->drawRect(Avatar::getX(), Avatar::getY(), 20, 20, TFT_RED);
	}
	if(active){
		if(blinkState){
			//strelice gore,dolje
		}else{
			//strelice gore,dolje
		}
	}
}

void EditableAvatar::setSelected(bool selected){
	EditableAvatar::isSelected = selected;
}

void EditableAvatar::setActive(bool active){
	EditableAvatar::active = active;
	if(active){
		Input::getInstance()->addListener(this);
	}else{
		Input::getInstance()->removeListener(this);
	}
}

uint8_t EditableAvatar::getAvatarIndex() const{
	return avatarIndex;
}

void EditableAvatar::loop(uint micros){
	blinkMicros += millis();
	if(blinkMicros > 200000){
		blinkState = !blinkState;
		blinkMicros = 0;
	}
}

void EditableAvatar::buttonPressed(uint id){
	switch(id){
		case btnUP:
			if(avatarIndex == 20){
				avatarIndex = 0;
			}else{
				avatarIndex++;
			}
			break;
		case btnDown:
			if(avatarIndex == 0){
				avatarIndex = 20;
			}else{
				avatarIndex--;
			}
			break;
	}
}
