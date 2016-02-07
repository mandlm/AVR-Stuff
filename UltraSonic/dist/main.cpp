#include "environment.h"

#include <avr/io.h>
#include <util/delay.h>

#include <stdio.h>

#include "ShiftRegister.h"
#include "KS0066.h"

int main(void)
{
	DDRC |= (1 << PC4) | (1 << PC3) | (1 << PC2);

	ShiftRegister shiftRegister(&PORTC, PC4, &PORTC, PC2, &PORTC, PC3);
	KS0066 lcd(shiftRegister);

	lcd.set("Ultra-sonic");

	DDRB |= (1 << PB0);		// PB0 trigger output
	DDRB &= ~(1 << PB1);	// PB1 pulse input

	// Timer 1, normal mode
	TCCR1A = 0;

	// Timer 1, clk/64 prescaler
	TCCR1B |= (1 << CS10) | (1 << CS11);

	_delay_ms(2000);
	lcd.clear();

	while (true)
	{
		uint8_t errorNum = 0;

		PORTB |= (1 << PB0);
		_delay_us(15);
		PORTB &= ~(1 << PB0);
		_delay_us(20);

		TCNT1 = 0;
		while ((PINB & (1 << PB1)) == 0 && errorNum == 0)
		{
			if (TCNT1 > 60000)
			{
				errorNum = 1;
				lcd.set("Timeout");
			}
		}

		if (errorNum == 0)
		{
			TCNT1 = 0;

			while ((PINB & (1 << PB1)) != 0 && errorNum == 0)
			{
				if (TCNT1 > 60000)
				{
					errorNum = 2;
					lcd.set("No object");
				}
			}

			if (errorNum == 0)
			{
				uint16_t ticks = TCNT1;

				uint16_t cm = (ticks * 1.835) / 20.0;

				char buffer[16];
				sprintf(buffer, "%3i cm", cm);
				lcd.set(buffer);
			}
		}

		_delay_ms(200);
	}
}

