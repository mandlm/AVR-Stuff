
#define F_CPU 4336180

#include <avr/io.h>
#include <util/delay.h>

class Pin
{
	public:
	volatile uint8_t *m_port;
	uint8_t m_pin;
	
	public:
	Pin(volatile uint8_t *port, uint8_t pin)
	: m_port(port)
	, m_pin(pin)
	{
		
	}
	
	void pulse()
	{
		*m_port |= (1 << m_pin);
		*m_port &= ~(1 << m_pin);
	}
	
	void set(bool value)
	{
		if (value == true)
		{
			*m_port |= (1 << m_pin);
		}
		else
		{
			*m_port &= ~(1 << m_pin);
		}
	}
	
	void toggle()
	{		
		*m_port ^= (1 << m_pin);
	}
};

class ShiftRegister
{
	Pin m_shiftClockPin;
	Pin m_dataPin;
	Pin m_latchClockPin;
	
	public:
	ShiftRegister(volatile uint8_t *shiftClockPort, uint8_t shiftClockPin,
	volatile uint8_t *dataPort, uint8_t dataPin,
	volatile uint8_t *latchClockPort, uint8_t latchClockPin)
	: m_shiftClockPin(shiftClockPort, shiftClockPin)
	, m_dataPin(dataPort, dataPin)
	, m_latchClockPin(latchClockPort, latchClockPin)
	{
	}
	
	void output(int8_t value)
	{
		for (int8_t bitIdx = 7; bitIdx >= 0; --bitIdx)
		{
			m_dataPin.set(value & (1 << bitIdx));
			m_shiftClockPin.pulse();
		}
		
		m_latchClockPin.pulse();
	}
};


int main(void)
{
	DDRA = (1 << PA0) | (1 << PA1) | (1 << PA3) | (1 << PA2);

	ShiftRegister shiftRegister(&PORTA, PA1, &PORTA, PA3, &PORTA, PA2);
	Pin ledPin(&PORTA, PA0);
	
	// lcd initialization goes here
	
	
    while(1)
    {
		ledPin.toggle();
		_delay_ms(125);
    }
}