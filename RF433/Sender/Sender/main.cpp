#define F_CPU 9600000 / 8

#include <avr/io.h>
#include <util/delay.h>

int main()
{
	DDRB |= (1 << PB4) | (1 << PB3);

	while (true)
	{
		PORTB |= (1 << PB3);
		PORTB |= (1 << PB4);
		_delay_ms(500);
		PORTB &= ~(1 << PB3);
		PORTB &= ~(1 << PB4);
		_delay_ms(500);
	}
}
