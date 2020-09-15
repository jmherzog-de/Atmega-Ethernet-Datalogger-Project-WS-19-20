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
 * @file messageView.c
 * @copyright Copyright 2019 Jean-Marcel Herzog. This project is released under the MIT license.
 * @date 25.11.2019
 * @version 1
 *
 * @brief Display view for messages
 *
 * This view display messages for the user.
 *
*/

#include "../ioconfig.h"
#include "../libs/gpio/gpio.h"
#include "../libs/lcd/lcd_lib.h"
#include "../libs/uart/uart.h"
#include "../libs/delay/delay.h"

#include "messageView.h"

/**
* @brief Display user defined message
*
* @note Message can't be longer then 16 characters.
*
* @param msg message to display
*
* @param pulseDelay pulse variable for delay (should be 10ms)
*
* @return void
*/
void messageView(char* msg, bool *pulseDelay)
{
	#ifdef DEBUG_MODE
	UART_puts(msg);
	UART_puts("\r\n");
	#endif
	
	lcd_clearDisplay();
	lcd_gotoxy(0,0);
	lcd_putstr(msg);
	delayPulseSteps(pulseDelay, 100);
}