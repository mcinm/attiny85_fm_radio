#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "OLED_I2C.h"
#include "soft_i2c.h"

#define START_POS_X 20
#define START_POS_Y 0


void set_frequency(double freq);
void update_screen(void);

volatile uint8_t set_status =0;
volatile uint8_t change=0;
volatile double hz = 102.0;


int main()
{
	sei();
	DDRB &= ~(1<<PB3);
	DDRB &= ~(1<<PB2);
	PORTB &= ~(1<<PB3) | ~(1<<PB2);
	DDRB &= ~(1<<PB4);
	PORTB |= (1<<PB4);
	MCUCR |= (1<<ISC00);
	GIMSK |= (1<<INT0);
	
	
	oled_init();
	oled_clear(1024);
	
	set_frequency(hz);
	update_screen();
	while(1)
	{
		if (change == 1) update_screen();
			
		if (!(PINB & (1<<PB4)))
		{
			if (set_status == 1)
			{
				set_frequency(hz);
				set_status = 0;
			}
		}
	}
}


void set_frequency(double freq)
{
	unsigned int frequencyB = 4* ((freq * 1000000) + 225000) / 32768;
	uint8_t frequencyH = frequencyB >> 8;
	uint8_t frequencyL = frequencyB & 0xFF;

	soft_i2c_start();
	soft_i2c_send(0xC0);
	soft_i2c_send(frequencyH);
	soft_i2c_send(frequencyL);
	soft_i2c_send(0xB0);
	soft_i2c_send(0x10);
	soft_i2c_send(0x00);
	soft_i2c_stop();
}

void hz_clear(void)
{
	oled_set_cursor(START_POS_X, START_POS_Y);
	for (uint8_t i=0; i<3; i++)
	{
		oled_clear(105);
		cursor.y += 1;
	}
}

void update_screen(void)
{
	hz_clear();
	oled_set_cursor(START_POS_X, START_POS_Y);
	oled_draw_double_number(hz);
	change = 0;
	set_status = 1;
}

// Interrupt handler
ISR(INT0_vect)
{
	static uint8_t dir;
	
	if (!(PINB & (1<<PB3)))
	{
		dir = (PINB & (1<<PB2));
	}
	else
	{
		if (dir != (PINB & (1<<PB2)))
		{
			if (dir) 
			{
				hz = hz - 0.1;
				
			}
			else 
			{
				hz = hz + 0.1;
				
			}
		}
	}
	change = 1;
}
