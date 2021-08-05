# attiny85 FM Radio based on TEA5767
Fm radio based on attiny85, TEA5767 and rotary encoder.

![attiny85_TEA5767](https://mcinm.pl/wp-content/uploads/2021/08/radio_fm-min-900x480.jpg)


![attiny85_fm_radio](https://mcinm.pl/wp-content/uploads/2021/07/schematic-radio_fm.png)

## There is error on schematic. Unpolarized capacitors are pF values


Attiny using software implementation of I2C protocol to communicate with fm receiver and OLED display.

Set clock frequency at 8MHz:
   "avrdude -c usbasp -p t85 -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m"

Build: make

Flash: make upload
