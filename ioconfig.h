/**
* Copyright 2019 Jean-Marcel Herzog
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
* associated documentation files (the "Software"), to deal in the Software without restriction,
* including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense
* and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do
* so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
* COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
* AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
* WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/**
 * @author: Herzog, Jean-Marcel
 * @file ioconfig.h
 * @copyright Copyright 2019 Jean-Marcel Herzog. This project is released under the MIT license.
 * @date 25.11.2019
 * @version 1
*/

#ifndef IOCONFIG_H_
#define IOCONFIG_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>

// definitions for lcd display
#define LCD_ENABLE	7			/**< Enable pin for LCD	*/

// definitions for UART0
#define BAUDRATE		9600		/**< UART baudrate								*/
#define UBRRH			UBRR0H		/**< UART baudrate register highbyte			*/
#define UBRRL			UBRR0L		/**< UART baudrate register lowbyte				*/
#define UCSRB			UCSR0B		/**< UART control and status register B			*/
#define RXEN			RXEN0		/**< activate recv UART messages				*/
#define TXEN			TXEN0		/**< activate transmit UART messages			*/
#define UCSRC			UCSR0C		/**< UART control and status register C			*/
#define UCSZ1			UCSZ01		/**< UART character size register 1				*/
#define UCSZ0			UCSZ00		/**< UART character size register 0				*/
#define UCSRA			UCSR0A		/**< UART control and status register B			*/
#define UDRE			UDRE0		/**< UART dataregister							*/
#define UDR				UDR0		/**< UART dataregister							*/
#define RXC				RXC0		/**< UART interrupt bit							*/

// definitions for SPI
#define SPI_DDR			DDRB		/**< SPI port data register						*/
#define SPI_PORT		PORTB		/**< SPI I/O port								*/
#define CS				PINB1		/**< chip select for sdcard module				*/
#define MOSI			PINB3		/**< MOSI pin									*/
#define MISO			PINB4		/**< MISO pin									*/
#define SCK				PINB5		/**< Clock pin									*/

// definitions for Timer0
#define TIMER_PREVAL	76		/**< Timer start value to generate 10ms pulse	*/

// definitions for ADC
#define ADC_CHANNEL		4			/**< ADC channel (PC4)							*/

#endif /* IOCONFIG_H_ */