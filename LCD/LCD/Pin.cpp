#include "Pin.h"

Pin::Pin(volatile uint8_t *port, uint8_t pin)
	: m_port(port)
	, m_pin(pin)
{

}

void Pin::pulse()
{
	*m_port |= (1 << m_pin);
	*m_port &= ~(1 << m_pin);
}

void Pin::set(bool value)
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

void Pin::toggle()
{
	*m_port ^= (1 << m_pin);
}

