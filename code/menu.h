#ifndef _MENU_LIB

	#include <inttypes.h>

	#include "config.h"
	#include "misc.h"

	#ifndef MENU_OFFSET
		#define MENU_OFFSET 0
	#endif

	typedef  struct{
		void (*handler_func)(void);
		// uint16_t (*get_value)(void);
	}menu_items;
	typedef menu_items *menu_items_p;

	extern menu_items menu[];
	extern uint8_t menu_selected;

	extern void (*menu_handler)(void);

	#define SET_MENU_HANDLER(F)	menu_handler=F
	#define CLEAR_MENU_HANDLER() menu_handler=NULL

	void poweroff(void);
	void menu_return(void);
	void calibration_intro(void);
	void menu_calibrate_capacity_min(void);
	void menu_calibrate_capacity_grip(void);
	void menu_calibrate_capacity_palm(void);
	void menu_animaton_select_next(void);
	void menu_animaton_select(void);
	void menu_animation_timestep_next(void);
	void menu_animation_timestep(void);
	void menu_animation_initial_state_setup(void);
	void menu_animaton_initial_state(void);
	void menu_animation_config(void);
	void menu_flight_length_setup(void);
	void menu_flight_length(void);
	void menu_idle_handler(void);
	void menu_touch_test_setup(void);

	#define _MENU_LIB
#endif
