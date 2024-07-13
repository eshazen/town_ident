
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

#include "kb.h"

uint8_t kb_val;			/* last read value */

void kb_init() {
  // switches are pull-up
  SW_PORT |= SW_MASK;
  ENC_SW_PORT |= ENC_SW_MASK;
}

// keyboard type-ahead buffer size, must be power of 2
#define KB_BUFFSIZ 4

static uint8_t kb_buff[KB_BUFFSIZ];
volatile uint8_t kb_head, kb_tail;

// advance pointer with wrap
#define kb_next(k) ((k+1)&(KB_BUFFSIZ-1))
// check for full
#define kb_full() (kb_next(kb_head)==kb_tail)
// check for empty
#define kb_empty() (kb_head==kb_tail)
// push to buffer
#define _push_kb(k) (kb_buff[kb_head]=k,kb_head=kb_next(kb_head))

// read current value
// 4 buttons (bits 0-3) and encoder switch (bit 4)
#define KB_BITS 5
#define read_kb() ((((SW_PIN >> SW0_BIT) & 0xf) | (((ENC_SW_PIN >> ENC_SW_BIT) & 1) << 4)) ^ 0x1f)

// return next code or 0
uint8_t get_kb() {
  uint8_t k;
  if( kb_empty())
    return 0;
  cli();
  k = kb_buff[kb_tail];
  kb_tail = kb_next(kb_tail);
  sei();
  return k;
}

// scan keyboard for hits
// zero if no hits, else an integer code
uint8_t scan_kb() {
  // local variables static to simplify
  static uint8_t kb_v;
  static uint8_t kb_d;
  static uint8_t kb_rc;
  static uint8_t kb_code;

  kb_rc = 0;

  kb_v = read_kb();		/* read current value */
  kb_d = kb_val ^ kb_v;		/* any changes? */
  if( kb_d) {
    for( uint8_t b=1, kb_code = 1; b<(1<<KB_BITS); b<<=1, kb_code++) {
      if( (kb_d & b) && (kb_v & b))	 /* only press, no release */
	kb_rc = kb_code;
    }
  }

  kb_val = kb_v;
  return kb_rc;
}


void poll_kb() {
  uint8_t k = scan_kb();
  if( k && !kb_full()) {
    _push_kb( k);
  }
}

