#include "SleepService.h"
#include <Input/Input.h>
#include <Loop/LoopManager.h>
#include <Chatter.h>
#include "../Interface/LVScreen.h"
#include "../Interface/LVModal.h"
#include "LoRaService.h"
#include "../Games/GameEngine/Game.h"
#include "../FSLVGL.h"
#include "../Screens/LockScreen.h"
#include <Settings.h>
#include <driver/rtc_io.h>

SleepService Sleep;

extern bool gameStarted;
extern Game* startedGame;

SleepService::SleepService(){

}

void SleepService::begin(){
	updateTimes();

	Input::getInstance()->addListener(this);
	LoopManager::addListener(this);

	resetActivity();
}

void SleepService::updateTimes(){
	sleepTime = SleepSeconds[Settings.get().sleepTime];
	shutdownTime = ShutdownSeconds[Settings.get().shutdownTime];
}

void SleepService::resetActivity(){
	activityTime = millis();
}

void SleepService::loop(uint micros){
	uint32_t elapsed = floor(((float) millis() - (float) activityTime) / (1000.0f)); // in seconds

	if(sleepTime && elapsed >= sleepTime){
		enterSleep();
	}
}

void SleepService::anyKeyPressed(){
	resetActivity();
}

void SleepService::enterSleep(){
	if(Chatter.backlightPowered()){
		Chatter.fadeOut();
	}

	LVScreen* screen;
	if(gameStarted){
		screen = startedGame->getGamesScreen();
		startedGame->stop();
		delete startedGame;
		gameStarted = false;
		startedGame = nullptr;
		FSLVGL::loadCache();
	}else{
		screen = LVScreen::getCurrent();
		screen->stop();
	}

	auto display = Chatter.getDisplay();
	display->getBaseSprite()->clear(TFT_BLACK);
	display->commit();

	const uint32_t sleepStart = millis();

resleep:
	while(LoRa.working){
		delay(10);
	}

	if(shutdownTime == 0){
		turnOff();
		ESP.restart(); // Just in case
	}

	gotMessage = false;
	Messages.addReceivedListener(this);

	// LoRa wakeup
	LoRa.radio.setDio1Action(LoRaService::moduleInterrupt);
	LoRa.radio.startReceive();
	esp_sleep_enable_gpio_wakeup();
	gpio_wakeup_enable((gpio_num_t) RADIO_DIO1, GPIO_INTR_HIGH_LEVEL);

	// Timer wakeup
	esp_sleep_enable_timer_wakeup((uint64_t) 2000000); // Wake every 2 seconds and check buttons

	// Sleep
	esp_light_sleep_start();

	// Awake here
	auto cause = esp_sleep_get_wakeup_cause();

	enum Reason { Radio, Timeout };
	const Reason reason = cause == ESP_SLEEP_WAKEUP_GPIO ? Radio : Timeout;

	if(reason == Timeout){
		Chatter.getInput()->loop(100);
		Chatter.getInput()->loop(100);

		bool press = false;
		for(int i = 0; i < 16; i++){
			press |= Chatter.getInput()->getButtonState(i);
			if(press) break;
		}

		if(!press){
			if(millis() - sleepStart >= shutdownTime * 1000){
				turnOff();
				ESP.restart(); // Just in case
			}else{
				goto resleep;
			}
		}
	}

	do {
		if(reason == Radio){
			delay(200);
		}else{
			delay(15);
		}
	}while(LoRa.working);
	for(int i = 0; i < 8; i++){
		Messages.loop(0);
	}

	Messages.removeReceivedListener(this);

	if(reason == Radio && !gotMessage){
		goto resleep;
	}

	LoopManager::resetTime();
	resetActivity();
	lv_timer_handler();

	LoopManager::defer([screen](uint32_t dt){
		LockScreen::activate(screen);
		lv_timer_handler();

		Chatter.fadeIn();
	});
}

void SleepService::turnOff(){
	if(Chatter.backlightPowered()){
		Chatter.fadeOut();
	}

	ledcDetachPin(PIN_BL);
	ledcDetachPin(PIN_BUZZ);

	digitalWrite(PIN_BL, 1);
	digitalWrite(PIN_BUZZ, 1);
	digitalWrite(CALIB_EN, 0);

	LoRa.radio.standby();
	LoRa.radio.sleep(false);
	esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
	esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
	esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
	esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
	esp_deep_sleep_start();
}

void SleepService::msgReceived(const Message& message){
	gotMessage = true;
}
