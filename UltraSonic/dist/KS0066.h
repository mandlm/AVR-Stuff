#pragma once

#include "environment.h"

#include <avr/io.h>

#include "ShiftRegister.h"

class KS0066
{
private:
	ShiftRegister *m_shiftRegister;

	static const uint8_t m_rs = 1;
	static const uint8_t m_rw = 2;
	static const uint8_t m_e = 3;
	static const uint8_t m_db4 = 4;
	static const uint8_t m_db5 = 5;
	static const uint8_t m_db6 = 6;
	static const uint8_t m_db7 = 7;

	uint8_t m_lines = 2;
	uint8_t m_columns = 16;

public:
	KS0066();
	KS0066(ShiftRegister &shiftRegister);

	void setShiftRegister(ShiftRegister &shiftRegister);

	void clear();

	void set(const char *string);


private:

	void initDisplay();

	void functionSet(bool interface8bit, bool twoLine, bool displayOn);
	void onOffControl(bool displayOn, bool cursorOn, bool blinkingOn);
	void entryMode(bool incrementMode, bool entireShift);
	void displayClear();

	void setCursorPos(uint8_t line, uint8_t column);

	void sendCommand(uint8_t data);

	void sendChar(char c);
	void sendString(const char *string);

	void sendByte(uint8_t data, bool isCommand);

	void execute(uint8_t registerValue);
};