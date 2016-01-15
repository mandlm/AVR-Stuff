#include "environment.h"

#include <avr/io.h>
#include <util/delay.h>

#include "Pin.h"
#include "ShiftRegister.h"
#include "KS0066.h"

int main(void)
{
	DDRA = (1 << PA0) | (1 << PA1) | (1 << PA3) | (1 << PA2);

	ShiftRegister shiftRegister(&PORTA, PA1, &PORTA, PA3, &PORTA, PA2);
	Pin ledPin(&PORTA, PA0);

	// lcd initialization goes here
	KS0066 lcd(shiftRegister);

	while(1)
	{
		ledPin.toggle();
		_delay_ms(125);
	}
}