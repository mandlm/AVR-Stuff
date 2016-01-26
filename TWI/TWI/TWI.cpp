#include "environment.h"

#include <avr/io.h>
#include <util/delay.h>

#include <avr/interrupt.h>
#include <util/twi.h>

#include "Pin.h"
#include "ShiftRegister.h"
#include "KS0066.h"

void I2C_init(uint8_t address)
{
	// load address into TWI address register
	TWAR = (address << 1);

	// set the TWCR to enable address matching and enable TWI, clear TWINT, enable TWI interrupt
	TWCR = (1<<TWIE) | (1<<TWEA) | (1<<TWEN);
}

void I2C_stop(void)
{
	// clear acknowledge and enable bits
	TWCR &= ~( (1<<TWEA) | (1<<TWEN) );
}

char displayBuffer[32];
volatile uint8_t displayBufferIdx = 0;
volatile bool writeDisplay = false;

ISR(TWI_vect)
{
	switch (TW_STATUS)
	{
	case TW_SR_SLA_ACK:			// own address has been acknowledged
		// clear TWI interrupt flag, prepare to receive next byte and acknowledge
		TWCR |= (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
		break;
	case TW_SR_DATA_ACK:
		displayBuffer[displayBufferIdx++] = TWDR;
		TWCR |= (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
		break;
	case TW_SR_STOP:
		writeDisplay = true;
		TWCR |= (1<<TWIE) | (1<<TWEA) | (1<<TWEN);
		break;
	default:
		// if none of the above apply prepare TWI to be addressed again
		TWCR |= (1<<TWIE) | (1<<TWEA) | (1<<TWEN);
	}
}

int main(void)
{
	DDRD |= (1 << PD6);
	DDRC |= (1 << PC4) | (1 << PC3) | (1 << PC2);

	Pin statusLed(&PORTD, PD6);
	statusLed.set(false);

	DDRD |= (1 << PD5);
	TCCR1A = (1<<WGM10) | (1<<COM1A1); // PWM, phase correct, 8 bit.
	TCCR1B = (1<<CS11) | (1<<CS10); // Prescaler 64 = Enable counter
	OCR1A = 0;

	DDRD &= ~(1 << PD3);
	PORTD |= (1 << PD3);

	ShiftRegister shiftRegister(&PORTC, PC4, &PORTC, PC2, &PORTC, PC3);

	KS0066 lcd(shiftRegister);

	statusLed.set(true);

	I2C_init(0x23);
	sei();

	while(true)
	{
		if (writeDisplay == true)
		{
			displayBuffer[displayBufferIdx] = 0;
			lcd.set(displayBuffer);
			writeDisplay = false;
			displayBufferIdx = 0;
		}

		if (!(PIND & (1 << PD3)))
		{
			OCR1A = ((OCR1A + 1) * 2) % 255;
			_delay_ms(300);
		}
	}
}