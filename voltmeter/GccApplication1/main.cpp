/*
 * GccApplication1.cpp
 *
 * Created: 9/18/2025 3:08:36 PM
 * Author : elhershberger21
 */ 




#include <avr/io.h>

#include <stdint.h>


void show_digits(int ones, int tens);


int main(void)

{
	DDRC = 0x00; //sets port c as inputs
	PORTC = 0xFF; //use the pull up resistors

	DDRD = 0x77; // Set Six Port F pins active output

	DDRB = 0x1F; // Set Six Port C pins active output including PC6 for button

	//PORTB |= (1<<PINB6);  // Activate the pull-up resistor for pin pC6

	int onesnum=0;

	int tensnum=0;

	TCCR1B |= (1<<CS10)|(1<<CS11);  // Set pre-scaling at 64

	TCNT1=0;

	
	int state = 0; 
	/* 
	state 0 = timer state;
	state 1 = display state;
	state 2 = wait state
	
	*/
	

	while (1)

	{

		if(bit_is_clear(PINC,0)){  //check to see if button is pressed
			
			while(bit_is_clear(PINC,0)){
				for(uint32_t n=0;n<300L; n++); // debounce
			}
			
			if(state == 1){
				onesnum = 0;  // reset the timer
				tensnum = 0;

			}
			state++;
			if(state == 3) state = 0;
		}

		for(int dly=0; dly<100; dly++)

		show_digits(onesnum, tensnum);
		
		if(state == 0){
			onesnum=onesnum+1;
		}

		if(onesnum==10){

			onesnum=0;

			tensnum=tensnum+1;

		}

		if(tensnum==10) tensnum=0;

	}

}









void show_digits(int ones, int tens){

	const uint8_t onesDigits [10] [5] =

	{

		{0x07, 0x05, 0x05, 0x05, 0x07},  //number 0

		{0x02, 0x06, 0x02, 0x02, 0x07},  //number 1

		{0x07, 0x01, 0x07, 0x04, 0x07},  //number 2

		{0x07, 0x01, 0x03, 0x01, 0x07},  //number 3

		{0x05, 0x05, 0x07, 0x01, 0x01},  //number 4

		{0x07, 0x04, 0x07, 0x01, 0x07},  //number 5

		{0x07, 0x04, 0x07, 0x05, 0x07},  //number 6

		{0x07, 0x01, 0x01, 0x01, 0x01},  //number 7

		{0x07, 0x05, 0x07, 0x05, 0x07},  //number 8

		{0x07, 0x05, 0x07, 0x01, 0x01}   //number 9

	};

	

	const uint8_t tensDigits [10] [5] =

	{

		{0x70, 0x50, 0x50, 0x50, 0x70},  //number 0

		{0x20, 0x60, 0x20, 0x20, 0x70},  //number 1

		{0x70, 0x10, 0x70, 0x40, 0x70},  //number 2

		{0x70, 0x10, 0x30, 0x10, 0x70},  //number 3

		{0x50, 0x50, 0x70, 0x10, 0x10},  //number 4

		{0x70, 0x40, 0x70, 0x10, 0x70},  //number 5

		{0x70, 0x40, 0x70, 0x50, 0x70},  //number 6

		{0x70, 0x10, 0x10, 0x10, 0x10},  //number 7

		{0x70, 0x50, 0x70, 0x50, 0x70},  //number 8

		{0x70, 0x50, 0x70, 0x10, 0x10}   //number 9

	};

	

	const uint8_t cathodes[5]={0x01, 0x02, 0x04, 0x08, 0x10};

	

	

	for(uint16_t row=0; row <=4; row++)

	{

		PORTB=~cathodes[row];

		PORTD=(onesDigits [ones] [row])|(tensDigits [tens] [row]);

		for(uint32_t n=0;n<300L; n++);

	}

	

}

