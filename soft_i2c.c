#include "soft_i2c.h"
#include <avr/io.h>
#include <util/delay.h>


void soft_i2c_init(void)
{
	SCLDDR |= (1<<SCL);
	SDADDR |= (1<<SDA);
	SCL_HIGH;
	SDA_HIGH;
}

void soft_i2c_start(void)
{
	SDA_LOW;
	_delay_us(1.3);
	SCL_LOW;
	_delay_us(0.6);
}

void soft_i2c_stop(void)
{
	SCL_HIGH;
	_delay_us(1.3);
	SDA_HIGH;
	_delay_us(1.3);
}


// send byte
unsigned char soft_i2c_send(unsigned char byte)
{	
	// Sending byte "bit by bit"
	for (unsigned char mask = 128; mask != 0; mask>>=1)
	{
		if (byte & mask) SDA_HIGH;
		else SDA_LOW;
		
		_delay_us(0.4);
		SCL_HIGH;
		_delay_us(1.1);
		SCL_LOW;
	}
	_delay_us(1);
	SDA_HIGH;
	SDADDR &= ~(1<<SDA);
	//_delay_us(1);
	SCL_HIGH;
	
	byte = SDAPIN & (1<<SDA);
	_delay_us(1.3);
	SCL_LOW;
	SDADDR |= (1<<SDA);
	
	_delay_us(1.3);
	SDA_LOW;
	return (byte == 0);
}


// get byte
unsigned char soft_i2c_read(void)
{
	unsigned char byte=0;
	unsigned char bit;
	
	SDA_HIGH;
	SDADDR &= ~(1<<SDA);
	
	for (unsigned char i=8; i>0; i--)
	{
		_delay_us(0.9);
		SCL_HIGH;
		bit = SDAPIN & (1<<SDA);
		byte <<= 1;
		if (bit) byte |= 1;
		_delay_us(0.9);
		SCL_LOW;
	}
	
	SDADDR |= (1<<SDA);
	SDA_LOW; // ACK
	
	_delay_us(0.8);
	SCL_HIGH;
	_delay_us(1.3);
	SCL_LOW;
	_delay_us(1.3);
	return byte;
}
