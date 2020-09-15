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
 * @file gpio.c
 * @copyright Copyright 2019 Jean-Marcel Herzog. This project is released under the MIT license.
 * @date 25.11.2019
 * @version 1
 *
 * @brief GPIO library
 *
 * @note A ioconfig.h file at root directory is required for this library.
 *
*/

#include "gpio.h"


/**
*
* @brief Read button state
*
* @param n button number of the MiniMexle (1...4)
*
* @return true: button pressed | false: button not pressed
*
*/
bool GPIO_GetSwitchState(uint8_t n)
{
	return !(PINC & (1<<(n-1)) );
}

/**
*
* @brief Set GPIO hardware pin to input
*
* @return void
*/
void GPIO_PrepareAsInput()
{
	DDRC = DDRC & 0xF0;
	PORTC = 0x0F;
	GPIO_CLR_BIT(PORTD, 7);
	_delay_us(10);
}

/**
*
* @brief Set GPIO hardware pin to output
*
* @return void
*
*/
void GPIO_DeactivateAsInput()
{
	DDRC = DDRC | 0x0F;
	PORTC = 0x00;
	_delay_us(10);
}