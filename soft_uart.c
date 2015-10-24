#define UART_BAUD_RATE	57600

#include <avr/io.h>
#include <util/parity.h>
#include <util/delay_basic.h>
#include "soft_uart.h"

#ifndef F_CPU
	#warning "You should define Clock Speed here"
	#define F_CPU			8e6
#endif

uint16_t pw = ( (double)F_CPU / ((double)4 * (double)UART_BAUD_RATE) ); // pulse width
UART_OPT uart_set = {1, 1, 8, UART_NO_PARITY}; // DEFAULT: port bit 1, stop bits 1, data size 8, no parity

void set_uart(UART_OPT set) {
	set._pp = 1 << (set._pp - 1);
	uart_set = set;
}

void uart_transmit(uint8_t data)
{
	register uint8_t p;
	
	// start bit {{
	UART_PORT &= ~uart_set._pp; // start bit, set to 0
	_delay_loop_2(pw - 8);
	// }}
	
	// data {{
	for(p=0;p<uart_set._data_size;p++) {
		if(!((data>>p) & 1)) UART_PORT &= ~uart_set._pp; // set to 0
		else UART_PORT |= uart_set._pp; // set to 1
		_delay_loop_2(pw - 10);
	}
	// }}
	
	// parity bit {{
	if(uart_set._parity_bit) {
		if(uart_set._parity_bit == UART_EVEN_PARITY) { // even parity
			if(parity_even_bit(data)) UART_PORT |= uart_set._pp; // set to 1
			else UART_PORT &= ~uart_set._pp; // set to 0
			_delay_loop_2(pw);
		}
		else if(uart_set._parity_bit == UART_ODD_PARITY) { // odd parity
			if(!parity_even_bit(data)) UART_PORT |= uart_set._pp; // set to 1
			else UART_PORT &= ~uart_set._pp; // set to 0
			_delay_loop_2(pw);
		}
	}	
	// }}
	
	// stop bit {{
	UART_PORT |= uart_set._pp; // stop bit, set to 1
	for(p=0;p<uart_set._stop_bits;p++) _delay_loop_2(pw);
	// }}
}

uint8_t uart_receive() {
	register uint8_t p, b = 0;
	
	// start bit {{
	while((UART_PIN & uart_set._pp)==0); // skip 0 level
	while((UART_PIN & uart_set._pp)==uart_set._pp); // skip 1 level
	_delay_loop_2(pw); _delay_loop_2(pw>>1);
	// }}
	
	// data {{
	for(p=0;p<uart_set._data_size;p++) {
		if((UART_PIN & uart_set._pp) == uart_set._pp) b |= (1 << p);
		_delay_loop_2(pw - 5);
	}
	// }}
	
	return b;
}
