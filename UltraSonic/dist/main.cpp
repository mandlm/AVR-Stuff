#include "environment.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <stdio.h>

#include "ShiftRegister.h"
#include "KS0066.h"
#include "Pin.h"

volatile bool measuring = false;
volatile int16_t dist = 0.0;

ISR(INT1_vect)
{
	if (PIND & (1 << PD3))
	{
		measuring = true;
		TCNT1 = 0;
	}
	else if (measuring == true)
	{
		dist = TCNT1 * 0.09175;

		measuring = false;
	}
}

ISR(TIMER1_COMPA_vect)
{
	measuring = false;
}

int main(void)
{
	DDRC |= (1 << PC4) | (1 << PC3) | (1 << PC2);

	ShiftRegister shiftRegister(&PORTC, PC4, &PORTC, PC2, &PORTC, PC3);
	KS0066 lcd(shiftRegister);

	// led ports to output
	DDRD |= (1 << PD6) | (1 << PD5);

	lcd.set("Ultra-sonic");

	DDRD |= (1 << PD4);		// PD4 trigger output
	DDRD &= ~(1 << PD3);	// PD3 pulse input

	// Timer 1, normal mode
	TCCR1A = 0;
	TCCR1B |= (1 << WGM12);		// CTC

	// Timer 1, clk/64 prescaler
	TCCR1B |= (1 << CS10) | (1 << CS11);

	// Timer 1, output compare interrupt 1A
	TIMSK |= (1 << OCIE1A);

	// Timer 1, compare value
	OCR1A = 30000;

	_delay_ms(500);
	lcd.clear();

	// INT1, on change
	GICR |= (1 << INT1);
	MCUCR |= (1 << ISC10);

	sei();

	while (true)
	{
		PORTD |= (1 << PD4);
		_delay_ms(25);
		PORTD &= ~(1 << PD4);
		_delay_ms(25);

		char buffer[16];
		sprintf(buffer, "%3i cm", dist);
		lcd.set(buffer);

		_delay_ms(250);
	}
}

