
#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"


void lcd_init( void)
{
  io_init();
  _delay_ms(15);
  send_ctrl( 0b0010);
  _delay_ms(5);

  send_ctrl(0b0010);   //Function set, 4 bit
  send_ctrl(0b1000);
 
  send_ctrl(0b0000);   //Display ON, Cursor On, Cursor Blinking
  send_ctrl(0b1111);

  send_ctrl(0b0000);   //Clear Display
  send_ctrl(0b0001);
 
  send_ctrl(0b0000);  //Entry Mode, Increment cursor position, No display shift
  send_ctrl(0b0110);
}

void lcd_cls( void)
{
  send_ctrl(0b0000);   //Clear Display
  send_ctrl(0b0001);

  lcd_addr( 0);
}
 


// unadjusted putc
void lcd_putch( char c) {
  volatile unsigned char z;	/* why? */
  z = (c >> 4) & 0xf;
  send_data(z);
  z = c & 0xf;
  send_data(z);
}

// putc to adjust low codes
void lcd_putc( char c) {
  volatile unsigned char z;	/* why? */
  if( c < 20)			/* adjust for codes 1-8 -> 0-7 */
    c--;
  z = (c >> 4) & 0xf;
  send_data(z);
  z = c & 0xf;
  send_data(z);
}

// set address.  0=home  40=2nd line
void lcd_addr( uint8_t addr)
{
  volatile uint8_t z;
  z = ((addr >> 4) & 7) | 8;
  send_ctrl( z);
  z = addr & 0xf;
  send_ctrl( z);
}

void lcd_puts( char *str) {
  volatile char *p = str;	/* why volatile? */
  int n = 0;
  
  while( *p) {
    lcd_putc( *p);
    ++p;
    if( ++n >= 16)
      return;
  }
}

void io_init( void)
{
  LCD_DATA_DDR |= LCD_DATA_MASK;
  LCD_CTRL_DDR |= _BV( LCD_EN_BIT) | _BV( LCD_RS_BIT);
} 

void send_ctrl( unsigned char __data) {
  LCD_DATA_PORT = (LCD_DATA_PORT & ~LCD_DATA_MASK) | (__data << LCD_DATA_D0_BIT); /* assert data */
  // set RS=L & EN=H
  LCD_CTRL_PORT &= ~ _BV(LCD_RS_BIT);
  LCD_CTRL_PORT |= _BV(LCD_EN_BIT);
  _delay_ms(1);
  // set EN=L
  LCD_CTRL_PORT &= ~ _BV(LCD_EN_BIT);
  _delay_ms(1);
}

void send_data( unsigned char __data) {
  LCD_DATA_PORT = (LCD_DATA_PORT & ~LCD_DATA_MASK) | (__data << LCD_DATA_D0_BIT); /* assert data */
  // set RS=H & EN=H
  LCD_CTRL_PORT |= _BV(LCD_EN_BIT) | _BV(LCD_RS_BIT);
  _delay_ms(1);
  // set EN=L
  LCD_CTRL_PORT &= ~ _BV(LCD_EN_BIT);
  _delay_ms(1);
}

