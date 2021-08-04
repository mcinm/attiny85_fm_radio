#define SCLDDR DDRB
#define SDADDR DDRB

#define SCLPORT PORTB
#define SDAPORT PORTB

#define SCLPIN PINB
#define SDAPIN PINB

#define SCL PB1
#define SDA PB0

#define SCL_HIGH SCLPORT |= (1<<SCL)
#define SCL_LOW SCLPORT &= ~(1<<SCL)

#define SDA_HIGH SDAPORT |= (1<<SDA)
#define SDA_LOW SDAPORT &= ~(1<<SDA)



void soft_i2c_init(void);
void soft_i2c_start(void);
void soft_i2c_stop(void);
unsigned char soft_i2c_send(unsigned char byte);
unsigned char soft_i2c_read(void);
