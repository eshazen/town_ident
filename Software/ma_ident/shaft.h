//
// Shaft encoder support
//


#ifndef _SHAFT_H_INCLUDED
#define _SHAFT_H_INCLUDED

// I/O ports/bits

// encoder A/B inputs
#define ENC_DDR DDRD
#define ENC_PORT PORTD
#define ENC_PIN PIND
#define ENC_A_BIT 2
#define ENC_B_BIT 3
#define ENC_MASK (_BV(ENC_A_BIT)|_BV(ENC_B_BIT))

void shaft_init();
uint8_t read_shaft();
uint8_t get_shaft();
uint16_t get_shaft_count();
void clear_shaft();
uint16_t get_speed();

#endif
