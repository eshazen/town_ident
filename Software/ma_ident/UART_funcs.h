#ifndef __UART_FUNCS_H__
#define __UART_FUNCS_H__
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>
#define PROMPT_CHARACTER '>'



#define USART_BAUDRATE 125000
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1) // system clock frequency (A MACRO THAT MUST BE DECLARED IN THE MAKE FILE) divided by the baudrate to find the baud prescale

//------------------------------------------------------------------------------
void UART_setup(void);

//------------------------------------------------------------------------------
void UART_newline(void);

//------------------------------------------------------------------------------
void UART_print(const char* const);

//------------------------------------------------------------------------------
void UART_flash_print(const char* const);

//------------------------------------------------------------------------------
void UART_print_noline(const char* const);

//------------------------------------------------------------------------------
void backspace_func(void);

//------------------------------------------------------------------------------
void UART_print_hex(uint64_t);

//------------------------------------------------------------------------------
void UART_print_hex(uint32_t);

//------------------------------------------------------------------------------
void UART_print_hex(uint16_t);

//------------------------------------------------------------------------------
void UART_print_hex(uint8_t);

//------------------------------------------------------------------------------
uint8_t UART_char_to_read_check(void);

//------------------------------------------------------------------------------
char UART_increment_interrupt_read();

#endif
