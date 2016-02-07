#include "ShiftRegister.h"

ShiftRegister::ShiftRegister(volatile uint8_t *shiftClockPort, uint8_t shiftClockPin,
                             volatile uint8_t *dataPort, uint8_t dataPin,
                             volatile uint8_t *latchClockPort, uint8_t latchClockPin)
	: m_shiftClockPin(shiftClockPort, shiftClockPin)
	, m_dataPin(dataPort, dataPin)
	, m_latchClockPin(latchClockPort, latchClockPin)
{

}

void ShiftRegister::output(int8_t value)
{
	for (int8_t bitIdx = 7; bitIdx >= 0; --bitIdx)
	{
		m_dataPin.set(value & (1 << bitIdx));
		m_shiftClockPin.pulse();
	}

	m_latchClockPin.pulse();
}

