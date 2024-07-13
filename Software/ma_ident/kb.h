#ifndef __KB_H__
#define __KB_H__

// pushbutton switches
#define SW_DDR DDRD
#define SW_PORT PORTD
#define SW_PIN PIND
#define SW0_BIT 4
#define SW1_BIT 5
#define SW2_BIT 6
#define SW3_BIT 7
#define SW_MASK 0xf0;

// encoder push switch input
#define ENC_SW_DDR DDRB
#define ENC_SW_PORT PORTB
#define ENC_SW_PIN PINB
#define ENC_SW_BIT 2
#define ENC_SW_MASK (_BV(ENC_SW_BIT))

void kb_init();
uint8_t scan_kb();
void push_kb( uint8_t k);
void poll_kb();
uint8_t get_kb();

#endif
