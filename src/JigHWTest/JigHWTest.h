#ifndef CHATTER_FIRMWARE_JIGHWTEST_H
#define CHATTER_FIRMWARE_JIGHWTEST_H

#include <Display/Display.h>
#include <Util/Vector.h>
#include <Input/Input.h>
#include <FS.h>
#include <Util/Task.h>

struct Test {
	bool (*test)();
	const char* name;
	void (*onFail)();
};

class JigHWTest
{
public:
	JigHWTest(Display* display);
	void start();

private:
	Sprite *canvas;
	Display *display;

	static JigHWTest* test;
	Vector<Test> tests;
	const char* currentTest;

	void log(const char* property, const char* value);
	void log(const char* property, float value);
	void log(const char* property, double value);
	void log(const char* property, bool value);
	void log(const char *property, uint32_t value);
	void log(const char *property, int32_t value);
	void log(const char *property, const String& value);

	static bool LoRaTest();
	static bool BatteryCheck();
	static bool SPIFFSTest();
	static bool hwRevision();
	/** UNUSED */
	static bool buttons();

	static uint32_t calcChecksum(fs::File& file);

	static constexpr int16_t USBVoltageMinimum = 3300;

	static constexpr float VoltReference = 2500;
	static constexpr float VoltReferenceTolerance = 100;
	static constexpr uint16_t MeasureCount = 10;
	static constexpr uint8_t Factor = 4;

	static constexpr uint8_t ButtonCount = 16;

	static constexpr uint8_t CurrentVersion = 1;
};

#endif //CHATTER_FIRMWARE_JIGHWTEST_H
