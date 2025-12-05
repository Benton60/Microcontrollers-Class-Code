#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	// Set PB1 (OC1A, Arduino Pin 9) as output
	DDRB |= (1 << PB1);
	TCCR1A = (1 << COM1A1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11) | (1 << CS10);  // Prescaler = 64


	ICR1 = 5000;  // 20 ms period

	while (1)
	{
		// Move to 0° (1.0 ms pulse): 1.0 ms / 4 µs = 250
		OCR1A = 133;
		_delay_ms(4000);

	
		OCR1A = 610;
		_delay_ms(4000);
	}
}


