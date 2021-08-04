
AVRDUDE_PROGRAMMER=USBasp
AVRDUDE_MCU=attiny85
 
MCU=attiny85
CC_FLAGS=-Os -Wall -Werror -mmcu=$(MCU) -DF_CPU=$(F_CPU) -c
LD_FLAGS=-mmcu=$(MCU)
 
F_CPU=8000000UL
CC=avr-gcc
CXX=avr-g++
LD=avr-gcc
OBJ_COPY=avr-objcopy

 
#OBJS=$(patsubst %.c, %.o, $(wildcard *.c), -%L, $(includes))
OBJS=$(patsubst %.c, %.o, $(wildcard *.c))

 
main.elf: $(OBJS)
	$(LD) $(LD_FLAGS) $^ -o $@
 
main.hex: main.elf
	$(OBJ_COPY) -O ihex -R .eeprom $< $@
 
%.o: %.c
	$(CC) $(CC_FLAGS) $< -o $@
 
%.o: %.c %.h
	$(CC) $(CC_FLAGS) $< -o $@
 
%.o: %.cpp
	$(CXX) $(CC_FLAGS) $< -o $@
 
%.o: %.cpp %.h
	$(CXX) $(CC_FLAGS) $< -o $@
 
upload: main.hex
	avrdude -c $(AVRDUDE_PROGRAMMER) -p $(AVRDUDE_MCU) -U flash:w:$<
 
clean:
	rm -rvf *.o main.elf main.hex
 
.PHONY: upload clean
