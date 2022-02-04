#ifndef CHATTER_FIRMWARE_LORASERVICE_H
#define CHATTER_FIRMWARE_LORASERVICE_H

#include <Arduino.h>
#include <SPI.h>
#include "LoRaPacket.h"
#include <Util/Task.h>
#include <RadioLib.h>
#include <queue>
#include <Sync/Mutex.h>
#include <Buffer/RingBuffer.h>
#include "SleepService.h"

class LoRaService {
friend SleepService;
public:
	LoRaService();
	bool begin();

	void send(UID_t receiver, LoRaPacket::Type type, const Packet* content);

	static void taskFunc(Task* task);

	ReceivedPacket<MessagePacket> getMessage();

	int32_t rand();
	int32_t rand(int32_t max);
	int32_t rand(int32_t min, int32_t max);
	UID_t randUID();

private:
	static const uint8_t PacketHeader[8];
	static const uint8_t PacketTrailer[8];

	LLCC68 radio;
	bool inited = false;

	void loop();

	std::queue<LoRaPacket> outbox;
	struct {
		std::queue<ReceivedPacket<MessagePacket>> message;
	} inbox;

	static portMUX_TYPE mux;
	volatile static bool available;
	RingBuffer inputBuffer;
	std::queue<LoRaPacket> received;
	static void moduleInterrupt();

	Mutex outboxMutex;
	Mutex inboxMutex;
	Mutex randomMutex;

	Task task;

	static const size_t randomSize = 24;
	std::queue<uint8_t> randos;

	void LoRaReceive();
	void LoRaProcessBuffer();
	void LoRaProcessPacket(LoRaPacket& packet);
	void LoRaProcessPackets();
	void LoRaSend();
	void LoRaRandom();

	/**
	 * Initialized RadioLib library without resetting the LoRa module.
	 */
	void initStateless();
};

extern LoRaService LoRa;

#endif //CHATTER_FIRMWARE_LORASERVICE_H
