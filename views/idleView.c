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
 * @file idleView.c
 * @copyright Copyright 2019 Jean-Marcel Herzog. This project is released under the MIT license.
 * @date 25.11.2019
 * @version 1
 *
 * @brief Display view for idle mode
 *
 * Datalogger ready to start view.
 *
*/


#include "idleView.h"
#include "../ioconfig.h"
#include "../libs/gpio/gpio.h"
#include "../libs/lcd/lcd_lib.h"

/**
*
* @brief Display view idle mode
*
* @return true: RUN | false: EXIT
*
*/
bool idleView()
{
	lcd_clearDisplay();
	
	_delay_ms(500); // give user time to release buttons

	lcd_gotoxy(0,0);
	lcd_putstr("      IDLE");
	lcd_gotoxy(1,0);
	lcd_putstr("RUN         EXIT");
	
	GPIO_PrepareAsInput();
	
	// read buttons
	while(1)
	{
		// EXIT button pressed?
		if(GPIO_GetSwitchState(4))
		{
			GPIO_DeactivateAsInput();
			return false;
		}
		// RUN button pressed?
		else if(GPIO_GetSwitchState(1))
		{
			GPIO_DeactivateAsInput();
			return true;
		}
	}
}