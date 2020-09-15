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
 * @file chooseSourceView.c
 * @copyright Copyright 2019 Jean-Marcel Herzog. This project is released under the MIT license.
 * @date 25.11.2019
 * @version 1
 *
* @brief Display view to choose target server configuration
*
* User can choose how the target server will be connected (hostname or ip-address)
*
*	Button functions:
*		-> Button 2: menu up
*		-> Button 3: menu down
*		-> Button 1 & Button 4: ok
 *
*/

#include "chooseSourceView.h"
#include "../ioconfig.h"
#include "../libs/gpio/gpio.h"
#include "../libs/lcd/lcd_lib.h"
#include "../libs/uart/uart.h"
#include "../libs/delay/delay.h"

/**
*
* @brief Choose target address or hostname
*
* @return 1: ip | 2: hostname
*
*/
uint8_t chooseSourceView()
{
	lcd_clearDisplay();
	
	uint8_t selected = 1;			// choosen menu item
	bool selectionChanged = false;	// flag for selection changed -> view will be updated
	
	lcd_gotoxy(0,0);
	lcd_putstr("[X] IP Adresse");
	lcd_gotoxy(1,0);
	lcd_putstr("[ ] Hostname");
	
	do{
		GPIO_PrepareAsInput();
		selectionChanged = 0;
		
		if(selected == 2 && GPIO_GetSwitchState(2))
		{
			selected = 1;
			selectionChanged = true;
		}
		else if(selected == 1 && GPIO_GetSwitchState(3))
		{
			selected = 2;
			selectionChanged = true;
		}
		else if(GPIO_GetSwitchState(4))
		{
			GPIO_DeactivateAsInput();
			break;
		}
		
		if(selectionChanged == true)
		{
			GPIO_DeactivateAsInput();
			if(selected == 1)
			{
				lcd_gotoxy(0,1);
				lcd_putc('X');
				lcd_gotoxy(1,1);
				lcd_putc(' ');
			} else if(selected == 2)
			{
				lcd_gotoxy(0,1);
				lcd_putc(' ');
				lcd_gotoxy(1,1);
				lcd_putc('X');
				
			}
			
			selectionChanged = false;
		}
	}
	while(1);
	
	return selected;
	
}