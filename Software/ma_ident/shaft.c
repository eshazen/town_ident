//
// Shaft encoder support
// Pin change interrupt on A/B inputs only
//
// uses 1kHz timer get_millis() for debounce
//

#include <avr/interrupt.h>
#include <avr/io.h>
#include "timer.h"
#include "shaft.h"
#include "kb.h"


volatile uint8_t shaft_state;
volatile uint8_t shaft_prev_state;
volatile int16_t shaft_count;
volatile uint32_t last_time;

// debounce delay
// #define DEBOUNCE_MS 2

void shaft_init() {
  // setup I/O pull-ups
  ENC_PORT |= ENC_MASK;
  ENC_SW_PORT |= ENC_SW_MASK;

  // setup for pin change interrupts on port D for A/B inputs
  PCICR |= _BV(PCIE2);
  PCMSK2 |= ENC_MASK;		// Port D bits

  last_time = get_millis();
}

uint8_t get_shaft() {
  return shaft_state;
}

uint16_t get_shaft_count() {
  return shaft_count;
}

// encoder ISR for port D (A/B inputs)

// encoder transition table
//     B  A  B0 A0
// 00  0  0  0  0   
// 01  0  0  0  1   -
// 02  0  0  1  0   +
// 03  0  0  1  1

// 04  0  1  0  0   +
// 05  0  1  0  1
// 06  0  1  1  0
// 07  0  1  1  1   -

// 08  1  0  0  0   -
// 09  1  0  0  1
// 0a  1  0  1  0
// 0b  1  0  1  1   +

// 0c  1  1  0  0
// 0d  1  1  0  1   +
// 0e  1  1  1  0   -
// 0f  1  1  1  1

ISR( PCINT2_vect) {

#ifdef DEBOUNCE_MS
  if( get_millis() - last_time < DEBOUNCE_MS)
    return;
#endif

  last_time = get_millis();

  shaft_state = read_shaft();
  switch( shaft_prev_state | (shaft_state << 2)) {
    // try only one tick per detent
    //  case 2:
    //  case 4:
    //  case 0xb:
  case 0xd:
    ++shaft_count;
    break;
    //  case 1:
    //  case 7:
    //  case 8:
  case 0xe:
    --shaft_count;
    break;
  default:
    break;
  }
  shaft_prev_state = shaft_state;
}


// read raw shaft state
//   bit 0 - A
//   bit 1 - B
//   bit 2 - switch

uint8_t read_shaft() {
  return( (((ENC_PIN & ENC_MASK) >> ENC_A_BIT) | 
	   (((ENC_SW_PIN & ENC_SW_MASK) >> ENC_SW_BIT) << 2)) ^ 7);
}



void clear_shaft() {
  shaft_count = 0;
}

