
#include <avr/io.h>

#include "led.h"

void led_init() {
  // LEDs are output
  LED_DDR |= LED_MASK;
  ENC_LED_DDR |= ENC_LED_MASK;
}

void set_leds( uint8_t v) {
  if( v & 1)			/* D1 */
    clear_io_bits( &LED_PORT, _BV(LED_BIT_D1));
  else
    set_io_bits( &LED_PORT, _BV(LED_BIT_D1));

  if( v & 2)			/* D2 */
    clear_io_bits( &LED_PORT, _BV(LED_BIT_D2));
  else
    set_io_bits( &LED_PORT, _BV(LED_BIT_D2));

  if( v & 4)			/* Green */
    set_io_bits( &ENC_LED_PORT, _BV(ENC_LED_BIT_GRN));
  else
    clear_io_bits( &ENC_LED_PORT, _BV(ENC_LED_BIT_GRN));    

  if( v & 8)			/* Red */
    set_io_bits( &ENC_LED_PORT, _BV(ENC_LED_BIT_RED));
  else
    clear_io_bits( &ENC_LED_PORT, _BV(ENC_LED_BIT_RED));

}

