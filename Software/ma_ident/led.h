#ifndef _LED_H_INCLUDED
#define _LED_H_INCLUDED

#include "ioport.h"

#define ENC_LED_DDR DDRC
#define ENC_LED_BIT_GRN 4
#define ENC_LED_BIT_RED 5
#define ENC_LED_PORT PORTC
#define ENC_LED_MASK (_BV(ENC_LED_BIT_GRN)|_BV(ENC_LED_BIT_RED))

#define LED_PORT PORTB
#define LED_DDR DDRB
#define LED_BIT_D1 0
#define LED_BIT_D2 1
#define LED_MASK (_BV(LED_BIT_D1)|_BV(LED_BIT_D2))

void led_init();
void set_leds( uint8_t v);

#endif
