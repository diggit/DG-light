#ifndef _TOUCH_LIB

	#include "config.h"

	typedef enum {FREE,PALM,GRIP}touch_status_t;
	extern volatile touch_status_t touch_status;



	#define CYCLE	5000
	#define SAMPLES	100

	extern volatile int32_t counter;
	//global
	#define EDGE_RISING		1
	#define EDGE_FALLING	2

	uint32_t touch_measure_capacity(uint8_t delay);
	void touch_init(void);

	#define _TOUCH_LIB
#endif
