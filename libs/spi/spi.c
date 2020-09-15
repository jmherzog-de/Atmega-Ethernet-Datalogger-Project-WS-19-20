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
 * @file spi.c
 * @copyright Copyright 2019 Jean-Marcel Herzog. This project is released under the MIT license.
 * @date 25.11.2019
 * @version 1
 *
 * @brief SPI library
 *
 * Access and control the SPI hardware of the uC.
 *
*/

#include "spi.h"

/**
*
* @brief SPI initialization
*
* SPI hardware initialization with frequency divider F_cpu / 16
*
* @return void
*
*/
void SPI_init()
{
	// port chip select for SDcard & Ethernet Controller, MOSI and SCK to output
	SPI_DDR |= (1<<CS) | (1 << PINB2) | (1<<MOSI) | (1<<SCK);
	
	// activate pull-up resistor for MISO
	SPI_DDR |= (1 << MISO);
	
	// activate SPI
	// define master moe and prescaler of f_cpu / 16
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0);
	
}

/**
*
* @brief Send/receive messages
*
* @param databyte data byte for transmit
*
* @return received data byte
*/
uint8_t SPI_transreceive(uint8_t databyte)
{
	// load data for transmit into register
	SPDR = databyte;
	
	// loop, unitl transmit has finished
	while( !(SPSR & (1<<SPIF)) );
	
	// return received data
	return SPDR;
}