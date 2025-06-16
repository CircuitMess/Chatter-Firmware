#include <SPIFFS.h>
#include <Battery/BatteryService.h>
#include <TypeDef.h>
#include <modules/LLCC68/LLCC68.h>
#include "JigHWTest.h"
#include "SPIFFSChecksum.hpp"
#include "../Services/BuzzerService.h"
#include <Pins.hpp>
#include <Chatter.h>
#include <Notes.h>
#include <Util/HWRevision.h>

JigHWTest *JigHWTest::test = nullptr;

JigHWTest::JigHWTest(Display* display) : display(display), canvas(display->getBaseSprite()){
	test = this;

	tests.push_back({JigHWTest::LoRaTest, "LoRa", [](){ }});
	tests.push_back({JigHWTest::BatteryCheck, "Bat check", [](){}});
	tests.push_back({JigHWTest::SPIFFSTest, "SPIFFS", [](){ }});
	tests.push_back({JigHWTest::buttons, "Buttons", [](){ }});
	tests.push_back({JigHWTest::hwRevision, "HW rev"});
}

void JigHWTest::start(){
	Serial.println();
	Serial.printf("TEST:begin:%llx\n", ESP.getEfuseMac());

	canvas->clear(TFT_BLACK);
	canvas->setTextColor(TFT_GOLD);
	canvas->setTextWrap(false, false);
	canvas->setTextDatum(textdatum_t::middle_center);

	canvas->setTextFont(0);
	canvas->setTextSize(1);
	canvas->setCursor(0, 6);

	canvas->printCenter("Chatter Hardware Test");
	canvas->setCursor(canvas->width() / 2, 16);
	canvas->println();
	display->commit();

	bool pass = true;
	for(const Test &test : tests){
		currentTest = test.name;

		canvas->setTextColor(TFT_WHITE);
		canvas->printf("%s: ", test.name);
		display->commit();

		Serial.printf("TEST:startTest:%s\n", currentTest);

		bool result = test.test();

		canvas->setTextColor(result ? TFT_SILVER : TFT_ORANGE);
		canvas->printf("%s\n", result ? "PASS" : "FAIL");
		display->commit();

		Serial.printf("TEST:endTest:%s\n", result ? "pass" : "fail");

		if(!(pass &= result)){
			if(test.onFail){
				test.onFail();
			}

			break;
		}
	}


	if(pass){
		Serial.println("TEST:passall");
	}else{
		Serial.printf("TEST:fail:%s\n", currentTest);
	}

	canvas->print("\n\n");
	canvas->setTextColor(pass ? TFT_BLUE : TFT_ORANGE);
	canvas->printCenter(pass ? "All OK!" : "FAIL!");
	display->commit();

	bool painted = false;
	const auto color = pass ? TFT_GREEN : TFT_RED;
	auto flashTime = 0;
	bool tone = false;
	const auto note = NOTE_C6 + ((rand() * 20) % 400) - 200;
	for(;;){
		if(millis() - flashTime >= 500){
			for(int x = 0; x < canvas->width(); x++){
				for(int y = 0; y <  canvas->height(); y++){
					if(!painted && canvas->readPixel(x, y) == TFT_BLACK){
						canvas->drawPixel(x, y, color);
					}else if(painted && canvas->readPixel(x, y) == color){
						canvas->drawPixel(x, y, TFT_BLACK);
					}
				}
			}

			flashTime = millis();
			painted = !painted;
			display->commit();
		}

		LoopManager::loop();
		auto press = false;
		for(int i = 0; i < ButtonCount; i++){
			if(Chatter.getInput()->getButtonState(i)){
				press = true;
				break;
			}
		}

		if(press && !tone){
			Piezo.tone(note);
			tone = true;
		}else if(!press && tone){
			Piezo.noTone();
			tone = false;
		}
	}
}

void JigHWTest::log(const char* property, const char* value){
	Serial.printf("%s:%s:%s\n", currentTest, property, value);
}

void JigHWTest::log(const char* property, float value){
	Serial.printf("%s:%s:%f\n", currentTest, property, value);
}

void JigHWTest::log(const char* property, double value){
	Serial.printf("%s:%s:%lf\n", currentTest, property, value);
}

void JigHWTest::log(const char* property, bool value){
	Serial.printf("%s:%s:%s\n", currentTest, property, value ? "TRUE" : "FALSE");
}

void JigHWTest::log(const char* property, uint32_t value){
	Serial.printf("%s:%s:%lu\n", currentTest, property, value);
}

void JigHWTest::log(const char* property, int32_t value){
	Serial.printf("%s:%s:%ld\n", currentTest, property, value);
}

void JigHWTest::log(const char* property, const String& value){
	Serial.printf("%s:%s:%s\n", currentTest, property, value.c_str());
}

bool JigHWTest::LoRaTest(){
	LLCC68 radio(new Module(RADIO_CS, RADIO_DIO1, RADIO_RST, RADIO_BUSY, Chatter.getSPILoRa()));
	int state = radio.begin(868, 500, 9, 5, RADIOLIB_SX126X_SYNC_WORD_PRIVATE, 22, 8, 0, false);

	if(state != RADIOLIB_ERR_NONE){
		test->log("begin", false);
		return false;
	}
	return true;
}

bool JigHWTest::BatteryCheck(){
	Battery.begin();
	uint16_t voltage = Battery.getVoltage();

	if(voltage < referenceVoltage - 100 || voltage > referenceVoltage + 100){
		test->log("voltage", (uint32_t) voltage);
		test->log("offset", Battery.getVoltOffset());
		return false;
	}

	return true;
}

bool JigHWTest::SPIFFSTest(){
	for(const auto& f : SPIFFSChecksums){
		if(!SPIFFS.exists(f.name)){
			test->log("missing", f.name);
			return false;
		}

		fs::File file = SPIFFS.open(f.name, "r");
		uint32_t sum = calcChecksum(file);
		file.close();

		if(sum != f.sum){
			test->log("file", f.name);
			test->log("expected", (uint32_t) f.sum);
			test->log("got", (uint32_t) sum);

			return false;
		}
	}

	return true;
}

uint32_t JigHWTest::calcChecksum(File& file){
	if(!file) return 0;

#define READ_SIZE 512

	uint32_t sum = 0;
	uint8_t b[READ_SIZE];
	size_t read = 0;
	while(read = file.read(b, READ_SIZE)){
		for(int i = 0; i < read; i++){
			sum += b[i];
		}
	}

	return sum;
}

bool JigHWTest::buttons(){
	auto input = Chatter.getInput();

	const auto cX = test->canvas->getCursorX();
	const auto cY = test->canvas->getCursorY();
	bool flash = false;
	uint32_t flashTime = 0;

	std::vector<bool> pressed(ButtonCount, false);
	std::vector<bool> released(ButtonCount, false);
	uint8_t pressCount = 0;
	uint8_t releaseCount = 0;
	for(;;){
		LoopManager::loop();

		for(int i = 0; i < ButtonCount; i++){
			if(input->getButtonState(i) && !pressed[i]){
				pressed[i] = true;
				pressCount++;
			}else if(!input->getButtonState(i) && pressed[i] && !released[i]){
				released[i] = true;
				releaseCount++;
			}
		}

		if(pressCount == ButtonCount && releaseCount == ButtonCount) break;

		if(millis() - flashTime > 500){
			if(flash){
				test->canvas->fillRect(cX, cY-4, 120, 8, TFT_BLACK);
			}else{
				test->canvas->setCursor(cX, cY);
				test->canvas->setTextColor(TFT_GOLD);
				test->canvas->printf("- PRESS BUTTONS -");
			}

			Chatter.getDisplay()->commit();
			flash = !flash;
			flashTime = millis();
		}

		test->canvas->fillRect(cX, cY+6, 120, 8, TFT_BLACK);
		test->canvas->setTextColor(TFT_LIGHTGRAY);
		test->canvas->setCursor(cX-3, cY+10);
		test->canvas->printf("[");
		for(int i = 0; i < ButtonCount; i++){
			if(input->getButtonState(i)){
				test->canvas->setTextColor(TFT_GOLD);
			}else if(pressed[i] && released[i]){
				test->canvas->setTextColor(TFT_BLUE);
			}else{
				test->canvas->setTextColor(TFT_DARKGRAY);
			}
			test->canvas->printf("-");
		}
		test->canvas->setTextColor(TFT_LIGHTGRAY);
		test->canvas->printf("]");

		Chatter.getDisplay()->commit();
	}

	test->canvas->fillRect(cX-3, cY-4, 120, 20, TFT_BLACK);
	test->canvas->setCursor(cX, cY);
	return pressCount == ButtonCount && releaseCount == ButtonCount;
}

bool JigHWTest::hwRevision(){
	const auto rev = HWRevision::get();
	if(rev != 0){
		test->canvas->printf("Fused: ");
		test->canvas->setTextColor(TFT_GOLD);
		test->canvas->printf("%d ", rev);
		test->canvas->setTextColor(TFT_WHITE);

		return rev == CurrentVersion;
	}

	HWRevision::write(CurrentVersion);
	HWRevision::commit();

	return true;
}

