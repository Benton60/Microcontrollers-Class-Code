/*
 * LabI2C.c
 *
 * Created: 8/31/2021 11:07:15 AM
 * Author : woolvertonk
 */ 


#define F_CPU 16000000L
#include <util/delay.h>
#include <avr/io.h>

void ERROR (void);
void send_start (void);
void send_stop (void);
void send_data (uint8_t datum);
void send_command (uint8_t cmnd);
void init_display(void);
void clear_display(void);
void send_info (uint8_t info);
void set_col_addr(uint8_t col_start, uint8_t col_stop);
void set_page_addr(uint8_t page_start, uint8_t page_stop);



int main(void)
{
    DDRB|=(1<<DDRB5);
	TWBR0=16;
	TWSR0 |= (1<<TWPS1);   //Adding a pre-scaler of 16 Gives SCL of 250 kHz

		
		
    init_display();
	
	clear_display();
	
	
	const uint8_t Trine_Logo[74]= {
		//first row 38 pixels wide 
		0x00, 0x02, 0x02, 0xFE, 0xFE, 0x82, 0x82, 0x82, 0x82, 0x82, 0xC6, 0x7C, 0x38, 0x00, 0x00, //B
		0x80, 0xC0, 0x60, 0x20, 0x20, 0x60, 0xC0, 0x80, 0x00, // e
		0x40, 0xE0, 0xE0, 0x40, 0x40, 0x20, 0x20, 0xE0, 0xC0, 0x00, 0x00, //n
		
		0x00, 
		//second
		0x00, 0x40, 0x40, 0x7F, 0x7F, 0x40, 0x40, 0x40, 0x40, 0x40, 0x41, 0x63, 0x3F, 0x1E, 0x00, //B
		0x1F, 0x3F, 0x71, 0x61, 0x61, 0x61, 0x21, 0x11, 0x00, //e
		0x40, 0x7F, 0x7F, 0x40, 0x00, 0x00, 0x40, 0x7F, 0x7F, 0x40, 0x00, //n
		0x00,
		
		
	};

	set_col_addr(0x2E, 0x51);
	set_page_addr(0x03, 0x04);
	for (int count=0; count<74; count++){
		send_info(Trine_Logo[count]);
	}
	

		


	
    while (1) 
    {
		clear_display();
		_delay_ms(500);
		set_col_addr(0x2E, 0x51);
		set_page_addr(0x03, 0x04);
		for (int count=0; count<74; count++){
			send_info(Trine_Logo[count]);
		}
		_delay_ms(500);
    }
}


void send_start ()
{
	PORTB &= 0xDF;
	TWCR0 = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);   //Send start condition.
	while (!(TWCR0 & (1<<TWINT)));   //Wait for TWINT flag set.  Indicates the START condition has been sent.
	if ((TWSR0 & 0xF8) != 0x08) //Check TWI status register.  If different from START (0x08 for send START) go to ERROR.
	ERROR();
	TWDR0 = 0x78;   //Load SLA_W into TWDR register.  Note SLA is 0x3C and write bit is 0.  SLA + W is 0x78.
	TWCR0 = (1<<TWINT)|(1<<TWEN);   //Clear TWINT bit in TWCR to start transmission of address.
	while (!(TWCR0 & (1<< TWINT)));   //Wait for SLA+W has been transmitted, and ACK/NACK has been received.
	if ((TWSR0 & 0xF8) != 0x18)   //Check value of TWI status register.  Mask pre-scaler bits (0x18 for the address being sent and ACK returned).  If status different from MT_SLA_ACK go to ERROR.
	ERROR();
}

void send_command (uint8_t y){
	send_start ();
	send_data (0x00);
	send_data (y);
	send_stop ();
}


void send_data (uint8_t x){
	TWDR0 = x ;   //Load DATA into TWDR register.  0x4C is an L in ASCII code.
	TWCR0 = (1<< TWINT)|(1<< TWEN);   //Clear TWINT bit in TWCR to start transmission of data.
	while (!( TWCR0 & (1<< TWINT)));   //Wait for TWINT flag set.  This indicates that the DATA has been transmitted, and ACK/NACK has been received.
	if ((TWSR0 & 0xF8) != 0x28)   //Check value of TWI status register.  Mask pre-scaler bits.  If status different from MT_DATA_ACK go to ERROR.  0x28 is data received and ACK sent.
	ERROR();
}


void send_stop (){
	TWCR0 = (1<< TWINT)|(1<<TWEN)|(1<<TWSTO);   //Transmit STOP condition
}



void init_display(){
	send_command(0xA8);  //Set MUX ratio
	send_command(0x3F);  //Set MUX ratio, 0x1F for 128x32 and 0x3F for 128x64
	send_command(0xD3);  //Set Display Offset
	send_command(0x00);  //Set Display Offset
	send_command(0x40);  //Set Display Start Line
	send_command(0x20);  //Set Display Mode to Horizontal
	send_command(0x00);  //Set Display Mode to Horizontal
	send_command(0xA1);  //Set Segment Re-map
	send_command(0xC8);  //Set COM Output Scan Direction
	send_command(0xDA);  //Set COM Pins hardware configuration
	send_command(0x12);  //Set COM Pins hardware configuration, 0x02 for 128x32 and 0x12 for 128x64
	send_command(0x81);  //Set Contrast Control
	send_command(0x9F);  //Set Contrast Control Value from 0x00 to 0xFF minimum to maximum
	send_command(0xA4);  //Disable Entire Display
	send_command(0xA6);  //Set Normal Display
	send_command(0xD5);  //Set Oscillation Frequency
	send_command(0x80);  //Set Oscillation Frequency
	send_command(0x8D);  //Enable Charge Pump Regulator
	send_command(0x14);  //Enable Charge Pump Regulator
	send_command(0xAF);  //Turn Display On
}


void clear_display(void){
	for (int count=0; count<1024; count++){
		send_info(0x00);
	}
}

void send_info (uint8_t y){
	send_start ();
	send_data (0x40);
	send_data (y);
	send_stop ();
}

void set_col_addr(uint8_t col_begin, uint8_t col_end){
	send_command(0x21);
	send_command(col_begin);
	send_command(col_end);
	
}

void set_page_addr(uint8_t page_begin, uint8_t page_end){
	send_command(0x22);
	send_command(page_begin);
	send_command(page_end);
}

void ERROR (){
	//PORTB |= (1<<PORTB5);
}




