#include <avr/eeprom.h>
#include <avr/io.h>
// #include <inttypes.h>
#include "storage.h"
#include "config.h"
#include "misc.h"
#include "animation.h"

#define ANIMATION_BASE	20

void storage_config_store(void)
{
	eeprom_update_byte(E8(0),ID);
	eeprom_update_block(&config,E8(1),sizeof(config_t));
}

void storage_all_store(void)
{
	storage_config_store();

	uint16_t address=ANIMATION_BASE;
	for(uint8_t a=0;a<ANIMATIONS_TOTAL;a++)
	{
		eeprom_update_word(E16(address),animations_flight[a].initial_state);
		address+=2;
		eeprom_update_byte(E8(address++),animations_flight[a].time_step);
		eeprom_update_byte(E8(address++),animations_flight[a].config);
	}
}

void storage_all_load(void)
{
	if(eeprom_read_byte(E8(0))==ID)
	{
		eeprom_read_block(&config,E8(1),sizeof(config_t));

		uint16_t address=ANIMATION_BASE;
		for(uint8_t a=0;a<ANIMATIONS_TOTAL;a++)
		{
			animations_flight[a].initial_state=eeprom_read_word(E16(address));
			address+=2;
			animations_flight[a].time_step=eeprom_read_byte(E8(address++));
			animations_flight[a].config=eeprom_read_byte(E8(address++));
		}
	}
	else
	{
		storage_all_store();
	}
}
