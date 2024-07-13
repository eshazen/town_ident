#ifndef __LCD_H__
#define __LCD_H__

//
// LCD Pinout
//      
// RS   - PB7
// R/W  - GND
// E    - PB6  
// D4   - PC0
// D5   - PC1
// D6   - PC2
// D7   - PC3
//

#define LCD_DATA_PORT PORTC
#define LCD_DATA_DDR DDRC
#define LCD_DATA_D0_BIT 0
#define LCD_DATA_MASK 0xf

#define LCD_CTRL_PORT PORTB
#define LCD_CTRL_DDR DDRB
#define LCD_EN_BIT 6
#define LCD_RS_BIT 7

void io_init( void);
void lcd_init( void);
void send_ctrl( unsigned char __data);
void send_data( unsigned char __data);
void lcd_puts( char *str);
void lcd_putc( char c);
void lcd_putch( char c);
void lcd_addr( uint8_t addr);
void lcd_cls( void);

#endif
