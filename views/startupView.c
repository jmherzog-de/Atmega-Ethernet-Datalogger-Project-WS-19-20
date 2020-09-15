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
 * @file startupView.c
 * @copyright Copyright 2019 Jean-Marcel Herzog. This project is released under the MIT license.
 * @date 25.11.2019
 * @version 1
 *
 * @brief Startscreen
 *
 * This view shows the user a startscreen with the current version of the datalogger software
 *
 * @note Important: this view implement the LCD panel.
*/

#include "startupView.h"
#include "../ioconfig.h"
#include "../libs/lcd/lcd_lib.h"
#include "../libs/delay/delay.h"


#ifdef DEBUG_MODE
#include "../libs/uart/uart.h"
#endif

uint16_t delayCount;

/**
* @brief Initialization of LCD
*/
void initDisplay()
{
	lcd_init();
}

/**
* @brief Show startscreen
*
* Display software version on panel.
*
* @return void
*
*/
void startupView()
{
	#ifdef DEBUG_MODE
	UART_puts("Ethernet data logger V0.1");
	#endif
	
	lcd_gotoxy(0,0);
	lcd_putstr("Ethernet");
	lcd_gotoxy(1,0);
	lcd_putstr("Datenlogger V0.1");
	
	return;
}