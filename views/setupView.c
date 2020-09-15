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
 * @file setupView.c
 * @copyright Copyright 2019 Jean-Marcel Herzog. This project is released under the MIT license.
 * @date 25.11.2019
 * @version 1
 *
 * @brief Show configuration menu
 *
 * User can choose between manual and automatic configuration with  SDcard.
 *
 * Button actions:
 *		-> Button 2: menu up
 *		-> Button 3: menu down
 *		-> Button 4: ok 
 *
*/

#include "setupView.h"
#include "../ioconfig.h"
#include "../libs/gpio/gpio.h"
#include "../libs/lcd/lcd_lib.h"

#ifdef DEBUG_MODE
#include "../libs/uart/uart.h"
#endif


/**
*
* @brief Setup menu view
*
* Selection between manual configuarion and SDcard configuration.
*
* @return 1: automatic config. | 2: manual config.
*
*/
uint8_t setupView(void)
{
	#ifdef DEBUG_MODE
	UART_puts("Display configuration menu \r\n");
	#endif
	
	lcd_clearDisplay();
	
	_delay_ms(500); // give user time to release buttons
	
	lcd_gotoxy(0,0);
	lcd_putstr("[X] SD Konfig.");
	lcd_gotoxy(1,0);
	lcd_putstr("[ ] Manuell");
	
	uint8_t selectedPosition = 1;	// choosen menu entry 1: SD 2: manual
	bool selectionChanged = false;	// flag for selection changed -> view will be updated
	
	do{
		
		GPIO_PrepareAsInput();
		selectionChanged = 0;
		
		if(selectedPosition == 2 && GPIO_GetSwitchState(2))
		{
			selectedPosition = 1;
			selectionChanged = true;
		}
		else if(selectedPosition == 1 && GPIO_GetSwitchState(3))
		{
			selectedPosition = 2;
			selectionChanged = true;
		}
		else if(GPIO_GetSwitchState(4))
		{
			GPIO_DeactivateAsInput();
			#ifdef DEBUG_MODE
			if(selectedPosition == 1)
			UART_puts("Automatic configuration chosen\r\n");
			else
			UART_puts("Manual configuration chosen\r\n");
			#endif
			
			return selectedPosition;
		}
		
		if(selectionChanged == true)
		{
			GPIO_DeactivateAsInput();
			
			if(selectedPosition == 1)
			{
				lcd_clearDisplay();
				lcd_gotoxy(0,0);
				lcd_putstr("[X] SD Konfig.");
				lcd_gotoxy(1,0);
				lcd_putstr("[ ] Manuell");
			}else if(selectedPosition == 2)
			{
				lcd_clearDisplay();
				lcd_gotoxy(0,0);
				lcd_putstr("[ ] SD Konfig.");
				lcd_gotoxy(1,0);
				lcd_putstr("[X] Manuell");
			}
			
			selectionChanged = false;
		}
		
	}while(1);
	
	return 0;
}