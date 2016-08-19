#include "button.h"
#include "animation.h"

#include <inttypes.h>
#include <avr/io.h>

//1223 ~ 3.27kHz
volatile uint8_t btn;//async
volatile uint16_t gptimer=0;

uint8_t btn_press_time=0;

void button_init(void)
{
	TIMSK|=(1<<TOIE0);
	TCNT0=0;
	TCCR0=(1<<CS02)|(1<<CS00);//1024 prescale
}


uint8_t button_read_click(void)
{
	if(bit_get(btn,FLAG_CLICK))
	{
		bit_clr(btn,FLAG_CLICK);
		return 1;
	}
	else
		return 0;
}

uint8_t button_read_hold(void)
{
	if(bit_get(btn,FLAG_HOLD))
	{
		bit_clr(btn,FLAG_HOLD);
		return 1;
	}
	else
		return 0;
}

volatile uint8_t blinker=0;

ISR(TIMER0_OVF_vect)
{
	if(status!=MENU)
	{
		blinker++;
		if(blinker>10)
		{
			blinker=0;
			STATUS_LED_OFF();
		}
		else if(blinker>5)
		{
			STATUS_LED_ON();
		}
	}

	if(gptimer)
	{
		gptimer++;
	}


	if(bit_get(PINB,(1<<BTN_PIN))==BTN_ACTIVE)
	{
		if(bit_get(btn,WAS)==0)
		{
			bit_set(btn,WAS);
			bit_set(btn,COUNTING);
		}
		if(bit_get(btn,COUNTING))
		{
			btn_press_time++;
			// STATUS_LED_OFF();

			if(btn_press_time>=BTN_HOLD_THRESHOLD)
			{
				bit_set(btn,FLAG_HOLD);
				bit_clr(btn,COUNTING);
				// menu_animation_initial_state();
				btn_press_time=0;
			}
		}
	}
	else
	{
		bit_clr(btn,WAS);
		bit_clr(btn,COUNTING);
		if(btn_press_time)
		{
			bit_set(btn,FLAG_CLICK);
			btn_press_time=0;
		}
	}
}
