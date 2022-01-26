#ifndef CHATTER_FIRMWARE_BATTERYELEMENT_H
#define CHATTER_FIRMWARE_BATTERYELEMENT_H

#include <Arduino.h>
#include "../LVObject.h"
#include <Chatter.h>

class BatteryElement : public LVObject, public LoopListener {
public:
	BatteryElement(lv_obj_t* parent);

	virtual ~BatteryElement();

	void loop(uint micros) override;

private:
	lv_obj_t* img;
	uint8_t level = 3;
	uint8_t newLevel = 0;
};


#endif //CHATTER_FIRMWARE_BATTERYELEMENT_H
