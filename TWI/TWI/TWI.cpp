#include "environment.h"

#include <avr/io.h>
#include <util/delay.h>

#include <avr/interrupt.h>
#include <util/twi.h>

#include "Pin.h"

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

volatile uint8_t dataBuffer;

ISR(TWI_vect)
{
	switch (TW_STATUS)
	{
	case TW_SR_SLA_ACK:			// own address has been acknowledged
		// clear TWI interrupt flag, prepare to receive next byte and acknowledge
		TWCR |= (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
		break;
	case TW_SR_DATA_ACK:
		dataBuffer = TWDR;
		TWCR |= (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
		break;
	case TW_ST_SLA_ACK:
		TWDR = dataBuffer + 1;
		TWCR |= (1<<TWIE) | (1<<TWINT) | (0<<TWEA) | (1<<TWEN);
		break;
	default:
		// if none of the above apply prepare TWI to be addressed again
		TWCR |= (1<<TWIE) | (1<<TWEA) | (1<<TWEN);
	}
}

int main(void)
{
	DDRD |= (1 << PD6);

	Pin statusLed(&PORTD, PD6);
	statusLed.set(false);

	I2C_init(0x23);
	sei();

	while(1)
	{
	}
}