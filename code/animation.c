#include <avr/io.h>
#include "animation.h"
#include "misc.h"
#include "button.h"
#include "touch.h"


uint16_t led_state=0;

void flush_ex(uint16_t leds)
{
	led_state=leds;
	leds=~leds;

	PORTC= (PORTC & ~PORTC_LEDS) | (leds & PORTC_LEDS);
	PORTB= (PORTB & ~PORTB_LEDS) | (leds & PORTB_LEDS);
	PORTD= (leds>>8);
}

uint16_t rotate_ex(uint16_t var, int8_t shift)
{
	uint16_t buffer=var;
	if(shift>0)
	{
		var=(var<<shift);
		var|=(buffer>>(16-shift));
	}
	else
	{
		shift=-shift;
		var=(var>>shift);
		var|=(buffer<<(16-shift));
	}
	return var;
}

uint16_t animation_counter=0;
animation_p animation=NULL;

#define SPIN_FLASHER_PERIOD	15
#define SPIN_FLASHER_TON	1
#define SPIN_FLASHER_TOFF	(SPIN_FLASHER_PERIOD-SPIN_FLASHER_TON)
void animation_spin_flasher(void)
{
	if(animation_counter%SPIN_FLASHER_PERIOD==0)
		flush_ex(1<<(animation_counter/SPIN_FLASHER_PERIOD));

	if((animation_counter+SPIN_FLASHER_TOFF)%SPIN_FLASHER_PERIOD==0)
		flush_ex(0);

	animation_counter++;
	if(animation_counter==16*SPIN_FLASHER_PERIOD)
		animation_counter=0;
}

void animation_spin_left(void)
{
	rotate(1);
	flush();
}
void animation_spin_right(void)
{
	rotate(-1);
	flush();
}


void animation_flasher(void)
{
	animation_counter++;
	if(animation_counter>=FLASHER_PERIOD)
		animation_counter=0;

	for(uint8_t i=0;i<FLASHER_COUNT;i++)
	{
		if(animation_counter==FLASHER_FLASH*i)
		 	flush_ex(0xFFFF);
		else if(animation_counter==FLASHER_FLASH*i+FLASHER_LIGHT)
			flush_ex(0);
	}
}

void animation_flasher_continous(void)
{
	animation_counter++;

	if(animation_counter==FLASHER_LIGHT)
		flush_ex(0);
	else if(animation_counter==FLASHER_FLASH)
	{
		flush_ex(0xFFFF);
		animation_counter=0;
	}
}

void animation_flight_length_test_oneshot(void)
{
	if(gptimer >= config.flight_length*FLIGHT_LENGTH_MULTIPLIER/16)
	{
		gptimer=1;
		if(animation_counter==16)
		{
			gptimer=0;
			animation_clear();
			// flush_ex(1<<(config.flight_length-1));
		}
		else
		{
			rotate(1);
			flush();
			animation_counter++;
		}
	}
}

void animation_touch_test(void)
{
	uint8_t i;
	if(counter>config.cap_min)
		i=crop_signed( (counter-config.cap_min) / ((config.cap_grip-config.cap_min)/16) ,0,15);
	else
		i=0;
	flush_ex(1<<i);
}


animation_t animations_flight[10]=
{
	/*
	void (*process)(void);
	uint16_t initial_state;
	uint8_t time_step;
	uint8_t config_step_size;
	uint8_t config;
	*/
	{
		.process=		animation_spin_left,
		.initial_state=	1,
		.time_step=		5,
		.config_step_size=2
		// config=
	},
	{
		.process=		animation_spin_right,
		.initial_state=	2,
		.time_step=		10,
		.config_step_size=2
	},
	{
		.process=		animation_flasher,
		.initial_state=	0,
		.time_step=		1,
		.config_step_size=1,
		.config=			CONFIG_NOINIT
	},
	{
		.process=		animation_flasher_continous,
		.initial_state=	0,
		.time_step=		1,
		.config_step_size=1,
		.config=			CONFIG_NOINIT
	},
	{
		NULL//sentinel
	}
};

animation_t animations_ground[]=
{
	{
		.process=		animation_flasher_continous,
		.initial_state=	0,
		.time_step=		10
	},
	{
		.process=		animation_flasher,
		.initial_state=	0,
		.time_step=		5
	},
	{
		.process=		animation_flight_length_test_oneshot,
		.initial_state=	1,
		.time_step=		1
	},
	{
		.process=		animation_spin_flasher,
		.initial_state=	0,
		.time_step=		20
	},
	{
		.process=		animation_touch_test,
		.initial_state=	0,
		.time_step=		10
	}
};

void animation_set(animation_p animation_sel)
{
	animation_counter=0;
	animation=animation_sel;
	flush_ex(animation->initial_state);
}
