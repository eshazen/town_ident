#include "utils.h"


volatile uint32_t micros_value = 0;

//------------------------------------------------------------------------------
//function to change upercase letters to lowercase, for comparing strings.
uint8_t to_lower(uint8_t letter)
{
  if((letter<91) && (letter>64)){ 
    return letter+0x20;
  }
  else{
    return letter;
  }
}

//------------------------------------------------------------------------------
void pin_setup(void){
  PORTD |= FPGA_PROGRAMMING_NCONFIG_PIN;
  //PORTD |= FPGA_PROGRAMMING_CONF_DONE_PIN;
  PORTD &= ~FPGA_PROGRAMMING_CLOCK_PIN;
  DDRD |= (FPGA_PROGRAMMING_CLOCK_PIN | FPGA_PROGRAMMING_NCONFIG_PIN); // set the flash programming pins as an output:
  PORTB |= FPGA_PROGRAMMING_DATA_PIN;
  DDRB |= (FPGA_PROGRAMMING_DATA_PIN | FLASH_CHIP_SELECT_PIN); //pinMode(DATA, OUTPUT);
  DDRC |= (FPGA_PROGRAMMING_TEST_PIN_1) | (LED_PIN);

  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1<<ADEN); // Set ADC prescalar to 128 and enable ADC
  
  ADMUX |= (1 << MUX2) | (1 << MUX0); //set ADC5
  ADMUX |= (1 << REFS0); // Set ADC reference to AVCC
  
  ADCSRA |= (1<<ADSC); //single conversion mode
}

//------------------------------------------------------------------------------
void timer_setup(void){
  //TCCR0A = 0;        // set entire TCCR1A register to 0
  TCCR0B = 0;
  
  // enable Timer0+1 overflow interrupt:
  TIMSK0 = (1 << TOIE0);
  TIMSK1 = (1 << TOIE1);
  // Set CS00 bit so timer runs at clock speed/8 ( 1 ms):
  TCCR0B |= (1 << CS01);
  // Set CS10 + 11 bits so timer runs at clock speed/64:
  TCCR1B |= (1 << CS10) | (1 << CS11);
}


//------------------------------------------------------------------------------
//function to send bytes to the fpga, with a clock
int send_bytes(uint8_t *data, int length){
  uint8_t *data_byte = data;
  uint8_t *data_end = data+length;
  while(data_byte < data_end){
    uint8_t local_data = *data_byte;
    for(uint8_t bit_mask = 1; bit_mask !=0; bit_mask <<= 1){
        PORTD &= ~FPGA_PROGRAMMING_CLOCK_PIN; // change the clock
      //--if the data changed, set portD to the new bit, otherwise keep it the same
      if(local_data & bit_mask){
        PORTB |= FPGA_PROGRAMMING_DATA_PIN; 
      }
      else{
        PORTB &= ~FPGA_PROGRAMMING_DATA_PIN; 
      }
      PORTD |= FPGA_PROGRAMMING_CLOCK_PIN; //change the clock again 
    }
    data_byte++;
  }  
}

//------------------------------------------------------------------------------
uint32_t get_micros(void){
  return micros_value;
}

//------------------------------------------------------------------------------
ISR(TIMER0_OVF_vect)
{
  micros_value++;
}
