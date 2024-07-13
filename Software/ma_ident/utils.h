#ifndef __UTILS_H__
#define __UTILS_H__
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define FPGA_PROGRAMMING_DATA_PIN 0x80 //DATA is out on pin B7 (--)
#define FPGA_PROGRAMMING_CLOCK_PIN 0x10 //DCLK is out on pin D4 (4)
#define FLASH_CHIP_SELECT_PIN 0x04 //slave select is pin B2 (10)
#define FPGA_PROGRAMMING_NCONFIG_PIN 0x20 //nConfig is on pin D5 (5)
#define FPGA_PROGRAMMING_INIT_DONE_PIN 0x10 //init_done is pin C4 (A4)
#define FPGA_PROGRAMMING_CONF_DONE_PIN 0x80 //CONF_DONE is on pin D7 (7)
#define FPGA_PROGRAMMING_NSTATUS_PIN 0x40 //nStatus is on pin D6 (6)
#define FPGA_PROGRAMMING_TEST_PIN_1 0x02 //test is pin C1 (A1)
#define TEMPERATURE_PIN 0x20 //temp sensor is on pin C5
#define LED_PIN 0x01 //led is on pin C0


//------------------------------------------------------------------------------

//function to change upercase letters to lowercase, for comparing strings.
uint8_t to_lower(uint8_t letter);

void pin_setup(void);

void timer_setup(void);

int send_bytes(uint8_t *data, int length);

uint32_t get_micros(void);

#endif
