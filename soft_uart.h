//*****************************************************************************
//
// File Name	: 'suart.h'
// Title		: UART Interface
// Author		: Ehsan Varasteh
// Created		: 2011-08-04
// Version		: 1.0
// Target MCU	: Atmel AVR series
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

//*****************************************************************************
// THESE CONSTANTS YOU SHOULD DEFINE YOURSELF {{
#ifndef UART_BAUD_RATE
	#define UART_BAUD_RATE		9600 // default
	/* some standard baud rates:
	57600 38400 19200 9600 4800 2400 1200 300 110
	*/
#endif

#ifndef UART_PORT
	#define UART_PORT	PORTC	// for transmit
#endif

#ifndef UART_PIN
	#define UART_PIN	PINC	// for receive
#endif
// }}
//*****************************************************************************

#define UART_NO_PARITY		0
#define UART_EVEN_PARITY	1
#define UART_ODD_PARITY		2

// System Errors
#define UART_ERROR_OVERRUN			100		// If the CPU does not service the UART quickly enough and the buffer becomes full, an Overrun Error will occur, and incoming characters will be lost.
#define UART_ERROR_UNDERRUN			101		// This error indication is commonly found in USARTs, since an underrun is more serious in synchronous systems.
#define UART_ERROR_FRAMING			102		// If the data line is not in the expected idle state when the "stop" bit is expected, a Framing Error will occur.
#define UART_ERROR_PARITY			103		// Parity error is set when the parity of an incoming data character does not match the expected value.
#define UART_ERROR_BREAK_CONDITION	104		// This error occurs when the receiver input is at the "space" level for longer than some duration of time, typically, for more than a character time.

typedef struct {
	unsigned char _pp; // port bit number, default = 1
	unsigned char _stop_bits; // stop bits, default = 1
	unsigned char _data_size; // data size, default = 8
	unsigned char _parity_bit; // 0:no, 1:EVEN, 2:ODD, default = 0
} UART_OPT; // UART Options


void set_uart(UART_OPT set); // set uart_settings
void uart_transmit(uint8_t data); // transmits 1 frame
uint8_t uart_receive(); // receives 1 frame