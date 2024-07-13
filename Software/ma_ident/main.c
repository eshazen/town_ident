/*
 * GPS code for SCOS control panel
 *
 * setup 1kHz timer interrupt
 * scan buttons every 8ms and queue hits
 * interrupt on shaft encoder rotation and update value
 * 
 * send to USART and 9600 baud:
 *   e <count>         on encoder change
 *   k <code>          on key press
 */

// display received UART data
#define UART_LCD

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <ctype.h>
#include <string.h>

#include "lcd.h"
// #include "shaft.h"
#include "timer.h"
// #include "kb.h"
#include "uart_int.h"
#include "led.h"

static char buff[500];

// static int16_t this_shaft;
// static uint8_t esc;

void delay_mul( int count) {
  if( count)
    for( int i=0; i<count; i++)
      _delay_ms( 25);
}

uint32_t ms;
uint8_t pos = 0;
uint8_t ch;
uint8_t leds = 0;

int main (void)
{
  lcd_init();
  //  shaft_init();
  timer_setup();
  //  kb_init();
  USART0Init();
  led_init();
  
  sei();

  lcd_cls();

  while( 1) {
    ms = get_millis();

    if( USART0GetErrors() & 1)
      leds |= 8;
    if( USART0GetErrors() & 2)
      leds |= 4;
    
    if( USART0CharacterAvailable()) {

      ch = USART0ReceiveByte();
      leds ^= 1;

      if( ch == '$') {
	leds ^= 2;
	lcd_addr( 0);
	lcd_putc( ch);

	USART0GetString( buff, sizeof(buff));

	if( !strncmp( buff, "GPGLL", 5)) {
	  lcd_cls();
	  lcd_puts( buff);
	  if( strlen( buff) > 16) {
	    lcd_addr( 40);
	    lcd_puts( buff+16);
	  }
	}
	

      }

      set_leds( leds);

    }
  }

//    uint8_t ch = USART0ReceiveByte();
//    lcd_putc( ch);
//    if( ++pos > 16) {
//      lcd_addr(0);
//      pos = 0;
//    }

//    if( USART0CharacterAvailable()) {
//       uint8_t ch = USART0ReceiveByte();
//       
//       if( ch == '$') {
//       }
//
//    }
//  }


}


