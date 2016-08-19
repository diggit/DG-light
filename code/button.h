#ifndef _BUTTON_LIB

	#include <avr/io.h>
	#include <avr/interrupt.h>

	#define BTN_PIN			4
	#define BTN_ACTIVE		0 //active low


	#define FLAG_CLICK	0x01
	#define FLAG_HOLD	0x02
	#define WAS			0x04
	#define COUNTING	0x08

	#define BTN_HOLD_THRESHOLD	20

	extern volatile uint8_t btn;//async
	extern volatile uint16_t gptimer;

	void button_init(void);
	uint8_t button_read_click(void);
	uint8_t button_read_hold(void);

	#define _BUTTON_LIB
#endif
