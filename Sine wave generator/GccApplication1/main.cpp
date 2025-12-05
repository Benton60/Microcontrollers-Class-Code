/*
 * GccApplication1.cpp
 *
 * Created: 10/30/2025 3:30:41 PM
 * Author : elhershberger21
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <Math.h>


uint16_t pulsewidthValues[100];

int index = 0;

void genTable(){
	for(uint8_t i = 0; i < 200; i++){
		double angle = 2.0 * M_PI / 200.0 * i;
		//                       change to change the amplitude
		double duty_cycle = .4 + .4 * sin(angle);
		
		pulsewidthValues[i] = (uint16_t)(duty_cycle * 1333);
	}
}


int main(void)
{
    genTable();
	
	
	
	/*
		TC1 is counting 6000 times a second generating a PWM with a duty length of .000166666 seconds
		then everytime it overflows OCR1A is being set to the next index in the table of duty cycle values
		
	
	*/
	//setting up timer counter 1
	DDRB |= (1<<PB1);
	
	
	//wgms set mode 14 
	// okay i have to switch to mode 8 NOT 14 becuase we need it to set the overflow once per not twice
	
	//cs10 sets prescale to 0;
	//com1a1 connects to output port b pin 1
	
	TCCR1A = (1 << WGM11)|(1 << COM1A1);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS10);
	//Enable the overflow interrupt
	TIMSK1 = (1 << TOIE1);

	
	//set the top of the count to 2667 so that it counts 6000 times a second
	ICR1 = 1333;
	OCR1A = pulsewidthValues[index++];
	
	sei();
	
    while (1) {}
}

ISR(TIMER1_OVF_vect) {
	OCR1A = pulsewidthValues[index++];
	if (index >= 200) index = 0;
}



//progress the sine wave displays but is jagged

