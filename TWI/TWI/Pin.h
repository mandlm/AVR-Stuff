#pragma once

#include <avr/io.h>

class Pin
{
private:
	volatile uint8_t *m_port;
	uint8_t m_pin;

public:
	Pin(volatile uint8_t *port, uint8_t pin);

	void pulse();
	void set(bool value);
	void toggle();

	void blink();
};
