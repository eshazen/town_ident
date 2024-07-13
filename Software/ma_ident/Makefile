#
# Makefile for blinky
#

MCU = atmega328

# 1 MHz for default oscillator
F_OSC = 8000000

# 16MHz for Stock arduino
# F_OSC = 16000000

# CFLAGS = -g -Os -mmcu=$(MCU) $(EXTRAOPT) -DF_CPU=$(F_OSC) -Wl,-u,vfprintf -lprintf_min -lm
# with float
CFLAGS = -g -Os -mmcu=$(MCU) $(EXTRAOPT) -DF_CPU=$(F_OSC) -Wl,-u,vfprintf -lprintf_flt -lm

LDFLAGS = -Wl,-Map=$(<:.o=.map),--cref,-u,vfprintf -mmcu=$(MCU) -lprintf_flt -lm

# lfuse default 0x62 write as 0xe2 to disable CLKDIV8
FUSEOPT = -U lfuse:w:0xe2:m

# read fuses
RFUSEOPT = -U hfuse:r:-:h -U lfuse:r:-:h -U efuse:r:-:h

# programmer configuration for avrdude programming utility
# for AVRISP programmer on USB
DUDEPORT  = -c avrispv2 -P usb

# USB tiny programmer
# DUDEPORT  = -c usbtiny -P usb

# for Arduino bootloader
# DUDEPORT  = -c arduino -P /dev/ttyACM0

DUDEMCU = m328p
AVRDUDE  = avrdude $(DUDEPORT) -p $(DUDEMCU) -y -u 

objects = main.o lcd.o shaft.o timer.o kb.o uart_int.o ioport.o led.o parsing.o

headers = lcd.h shaft.h timer.h kb.h uart_int.h ioport.h led.h parsing.h


all: $(objects)  main.hex list

clean:
	rm -f *.o *~ *.hex *.elf *.map *.lst

flash: main.hex
	$(AVRDUDE) -U flash:w:$^:i

list:
	avr-objdump -h -S main.elf > main.lst
	avr-size --common -x main.elf

$(objects): %.o: %.c $(headers)
	avr-gcc -c $(CFLAGS) $< -o $@

main.elf: $(objects)
	avr-gcc $(LDFLAGS) $(objects) -o $@

main.hex: main.elf
	avr-objcopy -j .text -j .data -O ihex $< $@

fuse:
	$(AVRDUDE) $(FUSEOPT)

rfuse:
	$(AVRDUDE) $(RFUSEOPT)
