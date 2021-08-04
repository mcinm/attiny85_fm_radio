#include "OLED_I2C.h"
#include "soft_i2c.h"
#include "font.h"

const unsigned char *font[10] = {m0,m1,m2,m3,m4,m5,m6,m7,m8,m9};

// SENDING COMMANDS
void oled_cmd(unsigned char cmd)
{
	unsigned char control_data = 0x00;
	soft_i2c_start();
	soft_i2c_send(oled_addr);
	soft_i2c_send(control_data);
	soft_i2c_send(cmd);
	soft_i2c_stop();
}
// SENDING DATA
void oled_data(unsigned char data)
{
	unsigned char control_data = 0x40;
	soft_i2c_start();
	soft_i2c_send(oled_addr);
	soft_i2c_send(control_data);
	soft_i2c_send(data);
	soft_i2c_stop();
}

// SET CURSOR
void oled_set_cursor(unsigned char x, unsigned char y)
{
	cursor.x = x;
	cursor.y = y;
}

// INIT SCREEN, I2C
void oled_init(void)
{
	soft_i2c_init();
	oled_cmd(0xAE);
    oled_cmd(0xD5);
    oled_cmd(0x80);
    oled_cmd(0xA8);
    oled_cmd(0x3F);
    oled_cmd(0xD3);
    oled_cmd(0x0);
    oled_cmd(0x40 | 0x0);
    oled_cmd(0x8D);
    oled_cmd(0x14);
    oled_cmd(0x20);
    oled_cmd(0x00);
    oled_cmd(0xA0 | 0x01);
    oled_cmd(0xC8);
    oled_cmd(0xDA);
    oled_cmd(0x12); // Jeżeli 128x32 - 0x02
    oled_cmd(0x81);
    oled_cmd(0xCF);
    oled_cmd(0xD9);
    oled_cmd(0xF1);
    oled_cmd(0xDB);
    oled_cmd(0x40);
    oled_cmd(0xA4);
    oled_cmd(0xA6);
    oled_cmd(0xAF);
}

// CLEAR DISPLAY
void oled_clear(unsigned short value)
{
	set_addressing();
	if (value == 1024)
	{
		oled_cmd(0x21);
		oled_cmd(0);
		oled_cmd(127);
		oled_cmd(0x22);
		oled_cmd(0);
		oled_cmd(7);
	}
	else
	{
		oled_cmd(0x21);
		oled_cmd(cursor.x);
		oled_cmd(cursor.x+value);
		oled_cmd(0x22);
		oled_cmd(cursor.y);
	}
	for (unsigned short i=0; i<value; i++) oled_data(0x00);
}

// DRAW CHARACTER
void oled_draw_character(unsigned char c)
{
	set_addressing();
	
	oled_cmd(0x21); // set column
	oled_cmd(cursor.x); // start-column
	oled_cmd(cursor.x+FONT_WIDTH);
	oled_cmd(0x22); // set page
	oled_cmd(cursor.y);
	
	for (unsigned char i=0; i<FONT_HIGH*FONT_WIDTH+3; i++)
		oled_data(pgm_read_byte(&font[c][i]));
	
	cursor.x += FONT_WIDTH + 2;
}

// DRAW DOT BETWEEN DIGITS
void oled_draw_dot(void)
{
	set_addressing();
	oled_cmd(0x21);
	oled_cmd(cursor.x);
	oled_cmd(cursor.x+7);
	oled_cmd(0x22);
	oled_cmd(cursor.y+2);
	
	for (unsigned char i=0; i<6; i++) oled_data(dot[i]);
	cursor.x += 10;
}

// DRAW DOUBLE NUMBER
void oled_draw_double_number(double number)
{
	unsigned short num1, num2;
	number = number * 10;
	num1 = (unsigned short)number;
	num2 = num1 % 10;
	num1 = num1 / 10;
	
	unsigned char buffer[3] = {};
	for(signed char i=2; i != -1; i--)
	{
		buffer[i] = num1 % 10;
		num1 = num1 / 10;
	}
	if (buffer[0] == 0) cursor.x += 10;
	for (unsigned char i=0; i<3; i++)
	{
		if (i != 0) oled_draw_character(buffer[i]);
		if (i == 0 && buffer[0] != 0) oled_draw_character(buffer[i]);
	}
	
	oled_draw_dot();
	oled_draw_character(num2);
	
	
	set_addressing();
	oled_cmd(0x21);
	oled_cmd(cursor.x+1);
	oled_cmd(cursor.x+12);
	oled_cmd(0x22);
	oled_cmd(cursor.y+2);
	for (unsigned char i=0; i<sizeof(HZ); i++) oled_data(HZ[i]);

}
