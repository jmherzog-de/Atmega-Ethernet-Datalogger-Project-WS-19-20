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
 * @file runView.c
 * @copyright Copyright 2019 Jean-Marcel Herzog. This project is released under the MIT license.
 * @date 25.11.2019
 * @version 1
 *
* @brief Display view datalogger active
*
* This view show up the user that the datalogger is active.
* A countdown display the user the time until next send.
*
* Tasterfunktion:
*	Taster 4 - Datenlogger stoppen
 *
*/

#include "runView.h"
#include "../libs/lcd/lcd_lib.h"
#include "../libs/adc/adc.h"
#include "../libs/gpio/gpio.h"

/**
*
* @brief Datalogger run view
*
* @param pulseSwitch view update pulse
*
* @param pulseRefresh button refresh pulse
*
* @param sec seconds until next send
*
* @return 0: send | 1: stop datalogger
*
*/
uint8_t runView(bool *pulseRefresh, bool *pulseSwitch, uint32_t *sec)
{
	lcd_clearDisplay();
	lcd_gotoxy(0,0);
	lcd_putstr("> RUNNING");

	lcd_gotoxy(1, 12);
	lcd_putstr("EXIT");

	uint8_t sw1_state;
	uint8_t pulse500cnt = 0;
	while(*sec > 0)
	{
		
		if(*pulseRefresh)
		{
			GPIO_PrepareAsInput();
			sw1_state = GPIO_GetSwitchState(4);
			if(sw1_state)
			{
				GPIO_DeactivateAsInput();
				return 1;
			}
			GPIO_DeactivateAsInput();
		}

		if(*pulseSwitch && pulse500cnt < 2)
		{
			pulse500cnt++;
		}
		
		if(*pulseSwitch && pulse500cnt == 2)
		{
			pulse500cnt = 0;
			
			// calculate ASCII values
			uint8_t tausend = 0;
			uint8_t hundert = 0;
			uint8_t zehner = 0;
			uint32_t sec_copy = *sec;
			while(sec_copy >= 1000)
			{
				tausend++;
				sec_copy-=1000;
			}
			while(sec_copy >= 100)
			{
				hundert++;
				sec_copy-=100;
			}
			while(sec_copy >= 10)
			{
				zehner++;
				sec_copy-=10;
			}
			lcd_gotoxy(1,0);
			lcd_putc(tausend+48);
			lcd_gotoxy(1,1);
			lcd_putc(hundert+48);
			lcd_gotoxy(1,2);
			lcd_putc(zehner+48);
			lcd_gotoxy(1,3);
			lcd_putc(sec_copy+48);

			*sec -= 1;
		}

		*pulseSwitch = false;
		*pulseRefresh = false;
	}

	return 0;
}