#pragma once

#include <avr/io.h>

#include "Pin.h"

class ShiftRegister
{
	Pin m_shiftClockPin;
	Pin m_dataPin;
	Pin m_latchClockPin;

public:
	ShiftRegister(volatile uint8_t *shiftClockPort, uint8_t shiftClockPin,
	              volatile uint8_t *dataPort, uint8_t dataPin,
	              volatile uint8_t *latchClockPort, uint8_t latchClockPin);

	void output(int8_t value);
};
