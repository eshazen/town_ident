#include "UART_funcs.h"
#include "utils.h"
#ifndef CIRCULAR_BUFFER_SIZE
#define CIRCULAR_BUFFER_SIZE 32
#endif

static char UART_circular_buffer[CIRCULAR_BUFFER_SIZE];
//const char* UART_circular_buffer_end = UART_circular_buffer+CIRCULAR_BUFFER_SIZE-1;
//char* read_position = UART_circular_buffer;
//char* write_position = UART_circular_buffer;
#define CIRCULAR_BUFFER_MASK 0x1F
static uint8_t iRead = 0;
static uint8_t iWrite= 0;
static uint8_t errorbit = 0;


//------------------------------------------------------------------------------
void UART_setup(void)
{ 
  memset(UART_circular_buffer,0,CIRCULAR_BUFFER_SIZE);

  //turn on the transmission and reception circutry
  UCSR0B |= (1 << RXEN0) | (1 << TXEN0); 
  UCSR0C |= ((1 << UCSZ00) | (1 << UCSZ01) | //8 //use 8 bit character sizes 
	     (0 << UMSEL01) | (0 << UMSEL00) | //Asynchronous USART
	     (0 << USBS0 )); // 1 stop bit
  UCSR0B |= (1 << RXCIE0 ); // Enable the USART Recieve Complete interrupt ( USART_RXC )
  
  // Load upper 8- bits of the baud rate value into the high byte of the UBRR register
  UBRR0H = ( BAUD_PRESCALE >> 8);
  // Load lower 8 - bits of the baud rate value into the low byte of the UBRR register
  UBRR0L = BAUD_PRESCALE; 
  //UART_print("Integrated TDC board v 0.1",);
  UART_print("");
  UDR0 = 0x3E; // print '>' character
  sei(); //enable global interrupts
  

}

//------------------------------------------------------------------------------
void UART_newline(void)
{
  while (( UCSR0A & (1 << UDRE0 )) == 0) {}; // Do nothing until UDR is ready for more data to be written to it
  UDR0 = 0xD; //newline
  while (( UCSR0A & (1 << UDRE0 )) == 0) {}; // Do nothing until UDR is ready for more data to be written to it
  UDR0 = 0xA; //carriage return
}
//------------------------------------------------------------------------------
void UART_print(const char* const printed_string)
{
  uint8_t letter_count = 0;
  while(printed_string[letter_count] != 0){
    while (( UCSR0A & (1 << UDRE0 )) == 0) {}; // Do nothing until UDR is ready for more data to be written to it
    UDR0 = printed_string[letter_count];
    letter_count++;
  }
  UART_newline();
}
//------------------------------------------------------------------------------
void UART_flash_print(const char* const printed_string)
{
  uint8_t letter_count = 0;
  while(pgm_read_byte(printed_string+letter_count) != 0){
    while (( UCSR0A & (1 << UDRE0 )) == 0) {}; // Do nothing until UDR is ready for more data to be written to it
    UDR0 = pgm_read_byte(printed_string+letter_count);
    while (( UCSR0A & (1 << UDRE0 )) == 0) {}; // Do nothing until UDR is ready for more data to be written to it
    letter_count++;
  }
  UART_newline();
}
//------------------------------------------------------------------------------
void UART_print_noline(const char* const printed_string)
{
  uint8_t letter_count = 0;
  while(printed_string[letter_count] != 0){
    while (( UCSR0A & (1 << UDRE0 )) == 0) {}; // Do nothing until UDR is ready for more data to be written to it
    UDR0 = printed_string[letter_count];
    letter_count++;
  }
}
//------------------------------------------------------------------------------
void backspace_func(void)
{
  while (( UCSR0A & (1 << UDRE0 )) == 0) {}; // Do nothing until UDR is ready for more data to be written to it 
 UDR0 = 0x8; //backspace
  while (( UCSR0A & (1 << UDRE0 )) == 0) {}; // Do nothing until UDR is ready for more data to be written to it
  UDR0 = 0x20; //space
  while (( UCSR0A & (1 << UDRE0 )) == 0) {}; // Do nothing until UDR is ready for more data to be written to it
  UDR0 = 0x8; //backspace
}

//------------------------------------------------------------------------------
void UART_print_hex(uint64_t longer_word)
{
  uint32_t word = 0xFFFFFFFF&(longer_word >> 32);
  UART_print_hex(word);
  word = 0xFFFFFFFF&(longer_word);
  UART_print_hex(word);
}

//------------------------------------------------------------------------------
void UART_print_hex(uint32_t long_word)
{
  uint8_t hex_char = 0xFF&(long_word>>24);
  UART_print_hex(hex_char);
  hex_char = 0xFF&(long_word>>16);
  UART_print_hex(hex_char);
  hex_char = 0xFF&(long_word>>8);
  UART_print_hex(hex_char);
  hex_char = 0xFF&(long_word);
  UART_print_hex(hex_char);
}

//------------------------------------------------------------------------------
//const char * const UART_return_hex_16(uint16_t med_word)
void UART_print_hex(uint16_t med_word)
{ 
  uint8_t hex_char = 0xFF&(med_word>>8);
  UART_print_hex(hex_char);
  hex_char = 0xFF&(med_word);
  UART_print_hex(hex_char);
}

//------------------------------------------------------------------------------
//const char * const UART_return_hex_8(uint8_t short_word)
void UART_print_hex(uint8_t short_word)
{
  const char* const Hex_chars[16] = {"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F"};
  uint8_t hex_char = short_word >> 4; //shift away all but the 4 most signifigant bits
  uint8_t char_num = hex_char & 0x0F;
  UART_print_noline(Hex_chars[char_num]); //print out the 4 most signifigant bits
  hex_char = short_word;
  char_num = hex_char & 0x0F; //print out the 4 least signifigant bits
  UART_print_noline(Hex_chars[char_num]);
}

//------------------------------------------------------------------------------
uint8_t UART_char_to_read_check()
{
  if(iWrite != iRead){
    return 1;
  }else{
    return 0;
  }
}

//------------------------------------------------------------------------------
ISR(USART_RX_vect)
{
  uint8_t new_iWrite = (iWrite + 1)&0x1F;//Find the next index for writing
  if(new_iWrite != iRead){ //check that we aren't hitting iRead
    UART_circular_buffer[iWrite] = UDR0;
    iWrite = new_iWrite; //update the write index
  } else { //buffer overflow
    errorbit = 1;
  }
}

//------------------------------------------------------------------------------
char UART_increment_interrupt_read()
{
  if(iRead != iWrite){ //check if there is a character to read
    char old_read_char = UART_circular_buffer[iRead]; 
    iRead=(++iRead)&0x1F; //update read index
    return old_read_char;
  }
  return -1;
}
