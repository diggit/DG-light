#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "touch.h"
#include "animation.h"
#include "misc.h"

volatile int32_t counter=0;
volatile touch_status_t touch_status=FREE;

uint32_t touch_measure_capacity(uint8_t delay)
{
	uint32_t buffer=0;
	uint8_t i;
	for(i=0;i<16;i++)
	{
		buffer+=counter;
		flush_ex(1<<i);
		delay_ms(delay);
	}
	flush_ex(0);
	return buffer/16;
}


void touch_init()
{
	// bit_set(DDRB,(1<<PB1));//OC1A output enable
	// edge=EDGE_RISING;//OUT is LOW, so it this initial cycle was like falling edge
	TIMSK|=(1<<OCIE1A);//enable interrupt on OCR1A COMPARE
	// TCCR1A=(1<<COM1A0)|(1<<COM1A1);//OCA1 to LOW on interrupt
	// TCCR1A=(1<<COM1A1);//OCA1 to HIGH on interrupt
	TCCR1B=(1<<WGM12)|(1<ICNC1);
	OCR1A=CYCLE/2;
	// sei();
	TCCR1B|=(1<<CS10);
}


//ISR only
uint8_t sample=0;
volatile uint8_t edge=EDGE_FALLING; //TODO: remove volatile?
int32_t isr_counter=0;

ISR(TIMER1_COMPA_vect)
{

	//counting
	isr_counter+=ICR1;
	if(sample<SAMPLES)
	{
		sample++;
	}
	else
	{
		counter=isr_counter;//strobe it out
		// STATUS_LED_FLIP();
		if(counter>=(config.cap_palm-config.cap_min)/3 + config.cap_min)
			touch_status=PALM;
		else if(counter>=(config.cap_grip-config.cap_min)/3 + config.cap_min)
			touch_status=GRIP;
		else
			touch_status=FREE;
		isr_counter=0;
		sample=0;
	}

	//propper OCA mode setup and ICR setup
	if(edge==EDGE_RISING)
	{
		edge=EDGE_FALLING;
		TCCR1A=(1<<COM1A1);//at the end of FALLING, switch OCA1 to HIGH
		bit_set(TCCR1B,(1<<ICES1));//capture when HIGH
	}
	else// if(edge==EDGE_FALLING)
	{
		edge=EDGE_RISING;
		TCCR1A=(1<<COM1A0)|(1<<COM1A1);//at the end of RISING, switch OCA1 to LOW
		bit_clr(TCCR1B,(1<<ICES1));//capture when LOW
	}
}
