#include "KS0066.h"

#include <util/delay.h>

KS0066::KS0066(ShiftRegister &shiftRegister) : m_shiftRegister(shiftRegister)
{
	initDisplay();
}

void KS0066::initDisplay()
{
	_delay_ms(30);

	functionSet(false, false, true);
	onOffControl(true, true, true);
	displayClear();
	entryMode(true, false);

	sendChar('i');
	sendChar('n');
	sendChar('i');
	sendChar('t');
}

void KS0066::functionSet(bool interface8bit, bool twoLine, bool displayOn)
{
	uint8_t command = 0b00100000;

	if (interface8bit)
	{
		command |= (1 << 4);
	}

	if (twoLine)
	{
		command |= (1 << 3);
	}

	if (displayOn)
	{
		command |= (1 << 2);
	}

	sendCommand(command);

	_delay_us(39);
}

void KS0066::onOffControl(bool displayOn, bool cursorOn, bool blinkingOn)
{
	uint8_t command = 0b00001000;

	if (displayOn)
	{
		command |= (1 << 2);
	}

	if (cursorOn)
	{
		command |= (1 << 1);
	}

	if (blinkingOn)
	{
		command |= (1 << 0);
	}

	sendCommand(command);

	_delay_us(39);
}

void KS0066::displayClear()
{
	uint8_t command = 0b00000001;

	sendCommand(command);

	_delay_ms(1.53);
}

void KS0066::entryMode(bool incrementMode, bool entireShift)
{
	uint8_t command = 0b00000100;

	if (incrementMode)
	{
		command |= (1 << 1);
	}

	if (entireShift)
	{
		command |= (1 << 0);
	}

	sendCommand(command);

	_delay_us(39);
}

void KS0066::sendCommand(uint8_t data)
{
	sendByte(data, true);
}

void KS0066::sendChar(char c)
{
	sendByte(c, false);

	_delay_us(43);
}

void KS0066::sendByte(uint8_t data, bool isCommand)
{
	uint8_t registerValue = 0;

	if (!isCommand)
	{
		registerValue |= (1 << m_rs);
	}

	if (data & (1 << 7))
	{
		registerValue |= (1 << m_db7);
	}

	if (data & (1 << 6))
	{
		registerValue |= (1 << m_db6);
	}

	if (data & (1 << 5))
	{
		registerValue |= (1 << m_db5);
	}

	if (data & (1 << 4))
	{
		registerValue |= (1 << m_db4);
	}

	execute(registerValue);

	registerValue &= (1 << m_rs);

	if (data & (1 << 3))
	{
		registerValue |= (1 << m_db7);
	}

	if (data & (1 << 3))
	{
		registerValue |= (1 << m_db6);
	}

	if (data & (1 << 1))
	{
		registerValue |= (1 << m_db5);
	}

	if (data & (1 << 0))
	{
		registerValue |= (1 << m_db4);
	}

	execute(registerValue);
}

void KS0066::execute(uint8_t registerValue)
{
	registerValue |= (1 << m_e);
	m_shiftRegister.output(registerValue);

	_delay_us(2);

	registerValue &= ~(1 << m_e);
	m_shiftRegister.output(registerValue);
}

