#define oled_addr 0x78 // write address

struct{
	unsigned char x;
	unsigned char y;
} cursor;



void oled_cmd(unsigned char cmd);
void oled_data(unsigned char data);
void oled_init(void);
void oled_clear(unsigned short value);
void oled_draw_character(unsigned char c);
void oled_draw_dot(void);
void oled_draw_double_number(double number);
void oled_set_cursor(unsigned char x, unsigned char y);

static inline void set_addressing(void)
{
	oled_cmd(0x20);
	oled_cmd(0x00);	
}
