#ifndef _CONFIG
	#define _CONFIG

	#include <avr/io.h>
	#include <inttypes.h>

	#define ID	0x15


	//freq of MCU clocking
	#define FOSC 8000000UL
	//
	// #define BAUD 9600UL
	// #define UART_TX
	// #define UART_RX
	// #define UART_RX_INT_EN

	#define PORTC_LEDS ((1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5))
	#define POTRC_OUTS 0
	#define PORTB_LEDS ((1<<6)|(1<<7))
	#define PORTB_OUTS ((1<<1)|(1<<2))
	#define PORTD_LEDS (0xFF)
	#define PORTD_OUTS 0

	typedef struct{
		uint32_t cap_min;
		uint32_t cap_grip;
		uint32_t cap_palm;
		uint8_t flight_length;
		uint8_t	animation;
		uint8_t arm_by_palm:1;//otherwise grip will arm animation
	} config_t;
	typedef enum {IDLE,ARMED,ANIMATING,MENU,FIND_ME,PICKED_UP} status_t;

	extern volatile status_t status;

	#define FLIGHT_LENGTH_MULTIPLIER	32

	#define MENU_OFFSET 8

	extern config_t config;

#endif
