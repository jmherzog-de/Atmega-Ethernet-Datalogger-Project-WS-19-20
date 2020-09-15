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
 * @file manconfigView.c
 * @copyright Copyright 2019 Jean-Marcel Herzog. This project is released under the MIT license.
 * @date 25.11.2019
 * @version 1
 *
* @brief Display view for manual configuration mode
*
* User can configure the target host parameter for the datalogger:
*
*	1) IP input
*
*	2) Port input
*
*	3) Interval input
*
*	Button functions:
*		-> selected position blink
*		-> Button 1: position left
*		-> Button 2: value up
*		-> Button 3: value down
*		-> Button 4: position right
*		-> Button 1 && Button 2: ok
 *
*/

#include "manconfigView.h"
#include "../ioconfig.h"
#include "../libs/gpio/gpio.h"
#include "../libs/lcd/lcd_lib.h"
#include "../libs/uart/uart.h"
#include "../libs/delay/delay.h"


uint16_t delayCount;

/**
*
* @brief Input ip address
*
* Activated position blink with a 500ms pulse
*
* @param paramIPAddress configured ip address
* @param pulseSwitch pulse for button read
* @param pulseRefresh pulse for view refresh
* @note IPv4 address configuration only
* @return void
*
*/
void setupIPView(uint8_t *paramIPAddress, bool *pulseSwitch, bool *pulseRefresh)
{
	
	lcd_clearDisplay();
	
	_delay_ms(500); // give user time to release buttons
	
	lcd_gotoxy(0,0);
	lcd_putstr("IP-Adresse:");
	
	bool on = true; // position flag
	
	uint8_t pos = 0; // position index of ip address(ip[])
	uint8_t ip[] = {
		0,0,0,
		0,0,0,
		0,0,0,
		0,0,0
	};
	
	bool sw1Old = false;
	bool sw1New = false;
	
	bool sw2Old = false;
	bool sw2New = false;
	
	bool sw3Old = false;
	bool sw3New = false;
	
	bool sw4Old = false;
	bool sw4New = false;
	
	do{
		
		//
		// read button states
		//
		if(*pulseRefresh)
		{
			GPIO_PrepareAsInput();
			
			sw1New = GPIO_GetSwitchState(1);
			sw2New = GPIO_GetSwitchState(2);
			sw3New = GPIO_GetSwitchState(3);
			sw4New = GPIO_GetSwitchState(4);
			
			// configure next parameter
			if(sw4New && sw1New) // sw4New && !sw4Old && sw1New && !sw1Old
			{
				GPIO_DeactivateAsInput();
				break;
			}
			
			// position left?
			if(sw1New && !sw1Old && pos > 0)
			pos--;
			
			// position right?
			if(sw4New && !sw4Old && pos < 11)
			pos++;
			
			// value up?
			if(sw2New && !sw2Old)
			{
				if(ip[pos] == 9)
				ip[pos] = 0;
				else
				ip[pos]++;
			}
			
			// value down?
			if(sw3New && !sw3Old)
			{
				if(ip[pos] == 0)
				ip[pos] = 9;
				else
				ip[pos]--;
			}
			
			sw1Old = sw1New;
			sw2Old = sw2New;
			sw3Old = sw3New;
			sw4Old = sw4New;
			
			GPIO_DeactivateAsInput();
			*pulseRefresh = 0;
		}
		
		
		//
		// display current values on screen
		// (activated position blinks)
		//
		
		if(*pulseSwitch && on)
		{
			uint8_t displayPosition = 0;
			uint8_t vecIndex = 0;
			for(uint8_t octet = 0; octet < 4; octet++)
			{
				for(uint8_t i = 0; i < 3; i++)
				{
					lcd_gotoxy(1,displayPosition++);
					
					if(vecIndex == pos)
					lcd_putc(' ');
					else
					lcd_putc(ip[vecIndex] + 48);
					
					vecIndex++;
				}
				if(octet == 3)
				continue;
				
				lcd_gotoxy(1,displayPosition);
				lcd_putc('.');
				displayPosition++;
			}
			
			*pulseSwitch = 0;
			on = false;
		}
		else if(*pulseSwitch && !on)
		{
			
			// print ip address
			uint8_t displayPosition = 0;
			uint8_t vecIndex = 0;
			for(uint8_t octet = 0; octet < 4; octet++)
			{
				for(uint8_t i = 0; i < 3; i++)
				{
					lcd_gotoxy(1,displayPosition++);
					lcd_putc(ip[vecIndex++] + 48);
				}
				if(octet == 3)
				continue;
				
				lcd_gotoxy(1,displayPosition);
				lcd_putc('.');
				displayPosition++;
			}
			
			sw1Old = sw1New;
			sw2Old = sw2New;
			sw3Old = sw3New;
			sw4Old = sw4New;
			
			*pulseSwitch = 0;
			on = true;
		}
		
	}
	while(1);
	
	// calculate ip address out of configured values
	paramIPAddress[0] = ( ip[0] * 100 + ip[1] * 10 + ip[2]   );
	paramIPAddress[1] = ( ip[3] * 100 + ip[4] * 10 + ip[5]   );
	paramIPAddress[2] = ( ip[6] * 100 + ip[7] * 10 + ip[8]   );
	paramIPAddress[3] = ( ip[9] * 100 + ip[10] * 10 + ip[11] );
	
	#ifdef DEBUG_MODE
	UART_puthex16(paramIPAddress[0]);
	UART_putc('.');
	UART_puthex16(paramIPAddress[1]);
	UART_putc('.');
	UART_puthex16(paramIPAddress[2]);
	UART_putc('.');
	UART_puthex16(paramIPAddress[3]);
	UART_puts("\r\n");
	#endif
	
	return;
}

/**
*
* @brief Port/interval input
*
* @param data variable for input value
* @param pulseSwitch pulse for button refresh
* @param pulseRefresh pulse for view refresh
* @return void
*/
void setupFourCharacters(uint8_t *data, bool *pulseSwitch, bool *pulseRefresh)
{
	bool on = 0;		// activated position flag
	uint8_t pos = 0;	// activated position index
	
	bool sw1Old = false;
	bool sw1New = false;
	
	bool sw2Old = false;
	bool sw2New = false;
	
	bool sw3Old = false;
	bool sw3New = false;
	
	bool sw4Old = false;
	bool sw4New = false;
	
	while(1)
	{
		if(*pulseRefresh)
		{
			GPIO_PrepareAsInput();
			
			sw1New = GPIO_GetSwitchState(1);
			sw2New = GPIO_GetSwitchState(2);
			sw3New = GPIO_GetSwitchState(3);
			sw4New = GPIO_GetSwitchState(4);
			
			
			// position left?
			if(sw1New && !sw1Old && pos > 0)
			pos--;
			
			// position right?
			if(sw4New && !sw4Old && pos < 3)
			pos++;
			
			// next input?
			if(sw4New && sw1New)
			{
				GPIO_DeactivateAsInput();
				return;
			}
			
			// value up?
			if(sw2New && !sw2Old)
			{
				if(data[pos] == 9)
				data[pos] = 0;
				else
				data[pos]++;
			}
			
			// value down?
			if(sw3New && !sw3Old)
			{
				if(data[pos] == 0)
				data[pos] = 9;
				else
				data[pos]--;
			}
			
			sw1Old = sw1New;
			sw2Old = sw2New;
			sw3Old = sw3New;
			sw4Old = sw4New;
			
			GPIO_DeactivateAsInput();
			*pulseRefresh = 0;
			
		}
		
		//
		// update screen
		// (activated position blinks)
		//
		if(*pulseSwitch && on)
		{
			for(uint8_t i = 0; i < 4; i++)
			{
				lcd_gotoxy(1,i);
				
				if(pos == i)
				lcd_putc(' ');
				else
				lcd_putc(data[i] + 48);
			}
			on = false;
			*pulseSwitch = 0;
		}
		else if(*pulseSwitch && !on)
		{
			
			for(uint8_t i = 0; i < 4;i++)
			{
				lcd_gotoxy(1,i);
				lcd_putc(data[i] + 48);
			}
			
			on = true;
			*pulseSwitch = 0;
		}
	}
}


/**
*
* @brief Input port
*
* @param paramPort Port
* @param pulseSwitch pulse for button refresh
* @param pulseRefresh pulse for view refresh
* @return void
*
*/
void setupPortView(uint32_t *paramPort, bool *pulseSwitch, bool *pulseRefresh)
{
	lcd_clearDisplay();
	lcd_gotoxy(0,0);
	lcd_putstr("Port:");
	
	// give user time to release buttons
	delayPulseSteps(pulseRefresh, 20);
	
	uint8_t port[] = {0,0,0,0};
	setupFourCharacters(port, pulseSwitch, pulseRefresh);
	
	// calculate port out of input
	*paramPort = port[0] * 1000 + port[1] * 100 + port[2] * 10 + port[3];
	
	return;
	
}


/**
*
* @brief Input interval
*
* @param paramInterval interval
* @param pulseSwitch pulse for button refresh
* @param pulseRefresh pulse for view refresh
* @return void
*
*/
void setupIntervalView(uint32_t *paramInterval, bool *pulseSwitch, bool *pulseRefresh)
{
	lcd_clearDisplay();
	lcd_gotoxy(0,0);
	lcd_putstr("Interval:");
	
	// give user time to release buttons
	delayPulseSteps(pulseRefresh, 20);
	
	uint8_t interval[] = {0,0,0,0};
	setupFourCharacters(interval, pulseSwitch, pulseRefresh);
	
	// calculate interval
	*paramInterval = interval[0] * 1000 + interval[1] * 100 + interval[2] * 10 + interval[3];
	return;
}


