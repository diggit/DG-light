#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>

#include "config.h"
#include "misc.h"
#include "storage.h"
#include "animation.h"
#include "menu.h"
#include "touch.h"
#include "button.h"
// #include "uart.h"

config_t config={0,0,0,100,0,0};

volatile status_t status=IDLE;


int main(void)
{
	uint8_t arm_lvl=0;
	touch_status_t local_touch;

	DDRC=PORTC_LEDS|POTRC_OUTS;
	DDRB=PORTB_LEDS|PORTB_OUTS;
	DDRD=PORTD_LEDS|PORTD_OUTS;
	PORTB|=(1<<BTN_PIN);
	flush_ex(0);
	STATUS_LED_OFF();

	sei();

	button_init();

	touch_init();

	if(bit_get(PINB,(1<<BTN_PIN))!=BTN_ACTIVE)
		storage_all_load();
	else
		storage_all_store();//store defaults

	while(1)
	{
		local_touch=touch_status;//create local copy to keep state same during whole cycle

		if( bit_get(btn,FLAG_HOLD) && status!=MENU)
		{
			status=MENU;
			menu_selected=0;
			bit_clr(btn,FLAG_HOLD);
			animation_clear();
			flush_ex(rotate_ex(1<<(menu_selected),MENU_OFFSET));
		}

		switch(status)
		{
			case IDLE:
				if((local_touch==PALM && config.arm_by_palm) || local_touch==GRIP)
				{
					arm_lvl++;

					delay_ms(50);
					if(arm_lvl==16)
					{
						status=ARMED;
						animation_set(&animations_ground[0]);
					}
				}
				else
				{
					arm_lvl=0;
				}
				flush_ex( FILL_BITS_FROM_LSB(arm_lvl) );
				break;

			case ARMED:
				switch(local_touch)
				{
					case FREE:
						animation_set(&animations_flight[config.animation]);
						gptimer=1;
						status=ANIMATING;
						break;
					case PALM:
						animation_clear();
						status=IDLE;
						break;

					default:
						break;
				}
				break;

			case ANIMATING:
				if(gptimer>=config.flight_length*FLIGHT_LENGTH_MULTIPLIER || local_touch==PALM) //grass touch is same as PALM touch...
				{
					gptimer=0;
					arm_lvl=0;
					animation_set(&animations_ground[1]);
					status=FIND_ME;
				}
				break;

			case MENU:
				if(menu_handler)
				{
					menu_handler();
				}
				else
				{
					STATUS_LED_ON();
					if(button_read_click())
					{
						if(menu_selected<15 && menu[menu_selected+1].handler_func!=NULL )
							menu_selected++;
						else
							menu_selected=0;
					}
					flush_ex(rotate_ex(1<<(menu_selected),MENU_OFFSET));

					if(button_read_hold())
					{
						if(menu[menu_selected].handler_func)
						{
							STATUS_LED_OFF();
							menu[menu_selected].handler_func();
						}
					}
					// if(menu_handler==NULL)//when menu handler was set just now, don't draw, it's up to him...

				}
				break;

			case FIND_ME://find me animation could be stopped by GRIP touch only
				if(local_touch==GRIP)
				{
					arm_lvl++;
					delay_ms(30);
					if(arm_lvl==16)
					{
						status=PICKED_UP;
						animation_set(&animations_ground[3]);

					}
				}
				else
				{
					arm_lvl=0;
				}
				break;

			case PICKED_UP://step between FIND_ME and ARMED, because both have same trigger (grip touch)
				if(local_touch==PALM)
				{
					status=IDLE;
					animation_clear();
				}
				break;



		}
		if(status==IDLE)// || status==ARMED || status== IDLE)
			delay_ms(1);//if IDLE has no delay, STATUS light blink is not regular, IDK WHY!

		if (animation!=NULL)
		{
			animation->process();
			delay_ms(animation->time_step);
		}
	}

}
