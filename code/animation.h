#ifndef _ANIMATION_LOB
	#include "config.h"
	#include "misc.h"

	#include <avr/io.h>

	#define STATUS_LED_PIN 2
	#define STATUS_LED_ON() bit_clr(PORTB,(1<<STATUS_LED_PIN))
	#define STATUS_LED_OFF() bit_set(PORTB,(1<<STATUS_LED_PIN))
	#define STATUS_LED_FLIP() bit_flip(PORTB,(1<<STATUS_LED_PIN))

	#define CONFIG_NOINIT	1

	void flush_ex(uint16_t leds);
	#define flush()	flush_ex(led_state)
	uint16_t rotate_ex(uint16_t var, int8_t shift);
	#define rotate(S)	led_state=rotate_ex(led_state,S)

	typedef struct{
		void (*process)(void);
		uint16_t initial_state;
		uint8_t time_step;
		uint8_t config_step_size;
		uint8_t config;
	}animation_t;

	typedef animation_t *animation_p;

	void animation_spin_left(void);
	void animation_spin_right(void);

	void animation_spin_flasher(void);

	#define FLASHER_PERIOD	500
	#define FLASHER_FLASH	50
	#define FLASHER_LIGHT	5
	#define FLASHER_COUNT	5
	void animation_flasher(void);
	void animation_flasher_continous(void);
	void animation_flight_length_test_oneshot(void);
	void animation_touch_test(void);

	extern animation_t animations_flight[];
	extern animation_t animations_ground[];
	extern animation_p animation;
	extern uint16_t animation_counter;
	void animation_set(animation_p animation_sel);
	#define animation_clear() animation=NULL;

	#define ANIMATIONS_TOTAL 10


	#define _ANIMATION_LOB
#endif
