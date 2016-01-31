#define F_CPU 12000000

#include <avr/io.h>
#include <util/delay.h>

int main()
{
	DDRB &= ~(1 << PB0);	// data input
	DDRD |= (1 << PD6);		// led output

	while (true)
	{
		if (PINB & (1 << PB0))
		{
			PORTD |= (1 << PD6);
		}
		else
		{
			PORTD &= ~(1 << PD6);
		}
		//_delay_ms(10);
	}
}

