# control panel demo

Simple software to implement a remote control panel.

Shaft encoder counts up on clockwise rotation, down on counter-clockwise
Pressing the shaft encoder button sets the count to zero

4 other buttons act as a keyboard with 4-character type ahead

Sends serial data at 38400 baud:

    e <value>    encoder value, updated on any change (signed 16-bit value)
    k <value>    key press value; 1-4 for buttons, 5 for encoder button
  
Received data is sent to LCD for display

    ^A clears screen and homes cursor
	^C moves cursor to start of 1st line
    ^B moves cursor to start of 2nd line
    ESC <char>   sets LEDs to low 4 bits of <char>
	             bit 0 - D1
				 bit 1 - D2
				 bit 3 - Encoder Green
				 bit 4 - Encoder Red

All printable ASCII codes are displayed at the cursor position


