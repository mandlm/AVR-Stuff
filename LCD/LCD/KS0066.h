#pragma once

#include <avr/io.h>

#include "ShiftRegister.h"

class KS0066
{
private:
	ShiftRegister &m_shiftRegister;

	static const uint8_t m_rs = 0;
	static const uint8_t m_rw = 1;
	static const uint8_t m_e = 2;
	static const uint8_t m_db4 = 3;
	static const uint8_t m_db5 = 4;
	static const uint8_t m_db6 = 5;
	static const uint8_t m_db7 = 6;

public:
	KS0066(ShiftRegister &shiftRegister);

private:
	void initDisplay();

	void functionSet(bool interface8bit, bool twoLine, bool displayOn);
	void onOffControl(bool displayOn, bool cursorOn, bool blinkingOn);
	void displayClear();
	void entryMode(bool incrementMode, bool entireShift);

	void sendCommand(uint8_t data);
	void sendChar(char c);

	void sendByte(uint8_t data, bool isCommand);

	void execute(uint8_t registerValue);
};