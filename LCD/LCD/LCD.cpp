
#define F_CPU 4336180

#include <avr/io.h>
#include <util/delay.h>

#include "Pin.h"
#include "ShiftRegister.h"

int main(void)
{
	DDRA = (1 << PA0) | (1 << PA1) | (1 << PA3) | (1 << PA2);

	ShiftRegister shiftRegister(&PORTA, PA1, &PORTA, PA3, &PORTA, PA2);
	Pin ledPin(&PORTA, PA0);

	// lcd initialization goes here
	LCD lcd(shiftRegister);

	while(1)
	{
		ledPin.toggle();
		_delay_ms(125);
	}
}