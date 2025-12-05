/*
 * GccApplication1.cpp
 *
 * Created: 9/11/2025 3:13:44 PM
 * Author : elhershberger21
 */ 

#include <avr/io.h>
#include <stdint.h>
#define COLUMN 0
#define PIXELS 0x03

int main(void)
{
	DDRD = 0X07; //set first three pins as outputs
	DDRB = 0X1F; //set first five pins as outputs
	PORTD  = PIXELS; //set the first two rows to high
	PORTB = ~(1<<COLUMN); //the first column will be set to low. this completes the circuit through the LEDs
    while (1) 
    {;
    }
}

