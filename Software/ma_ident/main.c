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

// display raw lat/lon for debug
// #define RAW_LATLON

// convert lat/lon to float and lookup town
#define TOWN_LATLON

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
#include "parsing.h"

static char buff[80];
static char latlon[80];
static char* toks[10];

static float lat_deg;
static float lon_deg;

#ifdef TOWN_LATLON
#include "places_ma.h"
static float dist;
static int inam;
static float t_lat;
static float t_lon;
static float t_dist;

static float p_lat;

#endif


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
uint8_t latlon_v = 0;

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

	if( !strncmp( buff+2, "GLL", 3)) {
	  strncpy( latlon, buff, sizeof(latlon));
	  latlon_v = 1;
	}
      }

      set_leds( leds);
    }

    if( latlon_v) {
      latlon_v = 0;
      int nt = split( latlon, toks, sizeof(toks)/sizeof(toks[0]));

#ifdef TOWN_LATLON
      if( nt >= 4 && *toks[2] == 'N' && *toks[4] == 'W') {
	// convert lat/lon
	lat_deg = conv_ll( toks[1]);
	lon_deg = -conv_ll( toks[3]);

	// find closest town
	dist = 1e10;
	inam = 0;
	strcpy( buff, "xxx");
	for( int i=0; i<NUM_PLACES; i++) {
	  t_lat = pgm_read_float( &lats[i]);
	  t_lon = pgm_read_float( &lons[i]);
	  t_dist = sqrt( (t_lat-lat_deg)*(t_lat-lat_deg)+(t_lon-lon_deg)*(t_lon-lon_deg) );
	  if( t_dist < dist) {
	    p_lat = t_lat;
	    dist = t_dist;
	    inam = i;
	  }
	}
	// print closest town
	lcd_cls();
	sprintf( buff, "%4.2f ", dist);
	lcd_puts( buff);
	strcpy_P( buff, (const char*) pgm_read_ptr( &(name_table[inam])));
	lcd_puts( buff);
	lcd_addr(40);
	sprintf( buff, "%7.3f,%7.3f", lat_deg, lon_deg);
	lcd_puts( buff);
      }
#endif      

#ifdef RAW_LATLON
      // display raw lat/lon as received for debug
      if( nt >= 4 && *toks[2] == 'N' && *toks[4] == 'W') {
	lcd_cls();
	lcd_puts( toks[1]);
	lcd_putc( ',');
	lcd_puts( toks[3]);
	lcd_putc( ' ');

	// convert lat/lon
	lat_deg = conv_ll( toks[1]);
	lon_deg = -conv_ll( toks[3]);
	lcd_addr(40);
	sprintf(buff, "[%8.3f,", lat_deg);
	lcd_puts( buff);
	sprintf(buff, "%8.3f]", lon_deg);
	lcd_puts( buff);
      }


#endif
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


