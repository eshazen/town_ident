//
// UART functions with interrupt-driven receiver
//

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "uart_int.h"

// must be a power of two less than 256
#ifndef CIRCULAR_BUFFER_SIZE
#define CIRCULAR_BUFFER_SIZE 1024
#endif

static char UART_circular_buffer[CIRCULAR_BUFFER_SIZE];
#define CIRCULAR_BUFFER_MASK (CIRCULAR_BUFFER_SIZE-1)
volatile static uint16_t iRead = 0;
volatile static uint16_t iWrite= 0;
static uint8_t errorbit = 0;

uint8_t USART0GetErrors() {
  return errorbit;
}

// initialize the UART.  Baud rate set in uart.h
void USART0Init(void)
{
  // Set baud rate
  UBRR0H = (uint8_t)(UBRR_VALUE>>8);
  UBRR0L = (uint8_t)UBRR_VALUE;
  // Set frame format to 8 data bits, no parity, 1 stop bit
  UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
  //enable transmission and reception
  UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
  UCSR0B |= (1 << RXCIE0 ); // Enable the USART Recieve Complete interrupt ( USART_RXC )
}


// send a string
void USART0SendString( char *s) {
  while( *s) {
    USART0SendByte( *s, NULL);
    ++s;
  }
}


// send a byte.  stream is provided for stdio compatibility
// and may be NULL
int USART0SendByte(char u8Data, FILE *stream)
{
  if(u8Data == '\n')
    {
      USART0SendByte('\r', stream);
    }
  //wait while previous byte is completed
  while(!(UCSR0A&(1<<UDRE0))){};
  // Transmit data
  UDR0 = u8Data;
  return 0;
}


// return true if a character is available for input
int USART0CharacterAvailable()
{
  return( iWrite != iRead);
}


//------------------------------------------------------------------------------
// receive a byte, block if none available
uint8_t USART0ReceiveByte()
{
  while( !USART0CharacterAvailable())
    ;
  char old_read_char = UART_circular_buffer[iRead]; 
  iRead=(++iRead) & CIRCULAR_BUFFER_MASK; //update read index
  return old_read_char;
}


//
// receive string from UART
// terminate on \r or \n (not stored)
// if more than max chars received, silently ignore them
//

void USART0GetString( char *buffer, int max)
{
  int n = 0;
  char *p = buffer;
  uint8_t c;
  
  while( 1) {
    c = USART0ReceiveByte();	   // receive char
    if( c == '\n' || c == '\r') {  // line ending?
      *p++ = '\0';                 // yes, terminate
      return;
    }

    ++n;			// increment count

    if( n < max)
      *p++ = c;			// only store if space left
    else			// flag overflow
      errorbit |= 2;
  }    
}



//------------------------------------------------------------------------------
ISR( USART_RX_vect)
{
  volatile uint16_t new_iWrite = (iWrite + 1) & CIRCULAR_BUFFER_MASK; //Find the next index for writing
  if(new_iWrite != iRead){ //check that we aren't hitting iRead
    UART_circular_buffer[iWrite] = UDR0;
    iWrite = new_iWrite; //update the write index
  } else { //buffer overflow
    errorbit |= 1;
  }
}
