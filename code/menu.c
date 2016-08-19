#include "menu.h"

#include <avr/sleep.h>

#include "animation.h"
#include "touch.h"
#include "storage.h"
#include "button.h"


menu_items menu[]=
{
	{
		menu_calibrate_capacity_min
	},
	{
		menu_calibrate_capacity_grip
	},
	{
		menu_calibrate_capacity_palm
	},
	{
		menu_animaton_select
	},
	{
		menu_animation_timestep
	},
	{
		menu_animaton_initial_state
	},
	{
		menu_flight_length
	},
	{
		menu_touch_test_setup
	},
	{
		poweroff
	},
	// {
	// 	menu_animation_config
	// },
	{
		NULL //sentinel
	}
};

void (*menu_handler)(void);


uint8_t menu_selected=0;

void menu_return(void)
{
	// menu_selected=0;
	CLEAR_MENU_HANDLER();
	animation_clear();
	storage_all_store();
	flush_ex(1<<menu_selected);
}

void poweroff(void)
{
	cli();
	DDRB=0;
	DDRC=0;
	DDRD=0;
	PORTB=0;
	PORTC=0;
	PORTD=0;
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	sleep_cpu();
}

void calibration_intro(void)
{
	for (uint8_t i = 0; i < 5; i ++)
	{
		delay_ms(300);
		flush_ex(0xFFFF);
		delay_ms(10);
		flush_ex(0);
	}
}

void menu_calibrate_capacity_min(void)
{
	calibration_intro();
	config.cap_min=touch_measure_capacity(100);
	menu_return();
}

void menu_calibrate_capacity_grip(void)
{
	calibration_intro();
	config.cap_grip=touch_measure_capacity(100);
	menu_return();
}

void menu_calibrate_capacity_palm(void)
{
	calibration_intro();
	config.cap_palm=touch_measure_capacity(100);
	menu_return();
}

void menu_animaton_select_next(void)
{
	if(button_read_click())
	{
		if(animations_flight[config.animation+1].process!=NULL )
		{
			config.animation++;
			if(animations_flight[config.animation+1].process==NULL ) //if next element would be first
				STATUS_LED_ON();
			else
				STATUS_LED_OFF();
		}
		else
			config.animation=0;

		animation_set(&animations_flight[config.animation]);
	}
	else if(button_read_hold())
	{
		menu_return();
	}
}

void menu_animaton_select(void)
{
	animation_set(&animations_flight[config.animation]);
	SET_MENU_HANDLER(menu_animaton_select_next);
}

#define MAX_TIME_STEPS	20

void menu_animation_timestep_next(void)
{
	if(button_read_click())
	{
		animation_p ani=&animations_flight[config.animation];
		uint8_t lvl;
		lvl=ani->time_step/ani->config_step_size+1;
		if(lvl==MAX_TIME_STEPS)
			STATUS_LED_ON();
		else
		{
			STATUS_LED_OFF();
			if(lvl>MAX_TIME_STEPS)
			{
				lvl=1;
			}
		}
		ani->time_step=ani->config_step_size*lvl;
	}
	else if(button_read_hold())
	{
		menu_return();
	}
}

void menu_animation_timestep(void)
{
	animation_set(&animations_flight[config.animation]);
	SET_MENU_HANDLER(menu_animation_timestep_next);
}

uint8_t config_buffer;
void menu_animation_initial_state_setup(void)
{
	animation_p ani=&animations_flight[config.animation];

	if(button_read_click())
	{
		config_buffer++;
		if(config_buffer==16)
		{
			flush_ex(0);
			menu_return();
		}
	}
	else if(button_read_hold())
	{
		bit_flip(ani->initial_state,1<<config_buffer);
	}

	if(bit_get(animation_counter,1))
	{
		flush_ex(1<<config_buffer);
	}
	else
	{
		flush_ex(ani->initial_state | (1<<config_buffer));
	}
	bit_flip(animation_counter,1);

	if(bit_get(ani->initial_state,1<<config_buffer))
		STATUS_LED_ON();
	else
		STATUS_LED_OFF();
}

void menu_animaton_initial_state(void)
{
	if(bit_get(animations_flight[config.animation].config,CONFIG_NOINIT))
		return;//initial state does not matter
	config_buffer=0;
	animation_counter=0;
	SET_MENU_HANDLER(menu_animation_initial_state_setup);
	animation_clear();
}

void menu_flight_length_setup(void)
{
	if(button_read_click())
	{
		config_buffer++;
		if(config_buffer>15)
			config_buffer=0;
		config.flight_length=config_buffer+1;
		gptimer=1;
		flush_ex(1<<(config_buffer));
		animation_counter=0;
		animation=&animations_ground[2];
	}
	else if(button_read_hold())
	{
		animation_clear();
		menu_return();
	}
}

void menu_flight_length(void)
{
	config_buffer=crop(config.flight_length,1,16)-1;
	flush_ex(1<<(config_buffer));
	gptimer=1;
	animation_counter=0;
	animation=&animations_ground[2];
	SET_MENU_HANDLER(menu_flight_length_setup);
}

void menu_idle_handler(void)
{
	if(button_read_hold())
	{
		animation_clear();
		menu_return();
	}
}
void menu_touch_test_setup(void)
{
	animation_set(&animations_ground[4]);
	SET_MENU_HANDLER(menu_idle_handler);
}

// void menu_animation_config(void)
// {
// 	;
// }
