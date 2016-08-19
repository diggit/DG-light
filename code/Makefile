export AVR_CC		= avr-gcc
AVR_LD			= $(AVR_CC)
AVR_OBJCOPY		= avr-objcopy
AVR_SIZE		= avr-size
MCU			= atmega8
export CFLAGS		= -c -Wunused -Wall -Wstrict-prototypes -Wundef -pedantic -g -$(OPTIMIZE) -mmcu=$(MCU) -funsigned-char -funsigned-bitfields -fpack-struct -fdata-sections -ffunction-sections -fdiagnostics-color=always -std=gnu99
# -mcall-prologues
OPTIMIZE		= O2
LDFLAGS			= -fdiagnostics-color=always -Wl,--gc-sections
TARGET			= main
SIZE_HISTORY		= size.log

export OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))

.NOTPARALLEL:

all: $(TARGET).hex

$(TARGET): compile_objects
	$(AVR_LD) -o $@.elf $(OBJECTS) $(LDFLAGS) -mmcu=$(MCU)

.PHONY: compile_objects
compile_objects:
	$(MAKE) -f Makefile.compile


%.hex: $(TARGET) size
	$(AVR_OBJCOPY) -j .text -j .data -O ihex $<.elf $@

.PHONY: clean
clean:
	rm -rf *.o $(TARGET).elf $(TARGET).hex

.PHONY: size
size: $(TARGET)
	@$(AVR_SIZE) -C --mcu=$(MCU) $(TARGET).elf

.PHONY: install
install: $(TARGET).hex
	avrdude -V -p m8 -c avrftdi -B 5 -U flash:w:$<
