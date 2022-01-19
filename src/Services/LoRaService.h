#ifndef CHATTER_FIRMWARE_LORASERVICE_H
#define CHATTER_FIRMWARE_LORASERVICE_H

#include <Arduino.h>
#include <SPI.h>
#include "LoRaPacket.h"
#include <Util/Task.h>
#include <RadioLib.h>
#include <queue>
#include <Sync/Mutex.h>

class LoRaService {
public:
	LoRaService();
	bool begin();

	void send(UID_t receiver, LoRaPacket::Type type, void* content, size_t size);

	static void taskFunc(Task* task);

	ReceivedPacket<MessagePacket> getMessage();

	int32_t rand();
	int32_t rand(int32_t max);
	int32_t rand(int32_t min, int32_t max);

private:
	static const uint8_t PacketHeader[4];

	LLCC68 radio;

	std::queue<LoRaPacket> outbox;
	struct {
		std::queue<ReceivedPacket<MessagePacket>> message;
	} inbox;

	Mutex outboxMutex;
	Mutex inboxMutex;

	Task task;

	void LoRaReceive();
	void LoRaSend();
};

extern LoRaService LoRa;

#endif //CHATTER_FIRMWARE_LORASERVICE_H