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
* @file showNetworkConfig.c
* @copyright Copyright 2019 Jean-Marcel Herzog. This project is released under the MIT license.
* @date 25.11.2019
* @version 1
*
* @brief Show network parameters of setup
*
* This view display all network parameters on the lcd with a pulse of 2s.
* ip address -> netmask -> gateway
*
*/

#include "showNetworkConfig.h"
#include "../ioconfig.h"
#include "../libs/delay/delay.h"
#include "../libs/lcd/lcd_lib.h"
#include "../libs/gpio/gpio.h"


/**
*
* @brief Show ip address
*
* Convert ip address from integer to ASCII character and
* display them on the lcd.
*
* @param ip ip-address array (every ip octet have to be a array element)
*
* @return void
*
*/
void showDeviceIP(uint8_t *ip)
{
	lcd_clearDisplay();
	lcd_gotoxy(0,0);
	lcd_putstr("IP-Adresse:");
	
	uint8_t octet;			// copy of the current ip octet
	uint8_t hundert = 0;	// value of 100's in ip octet
	uint8_t zehner = 0;		// value of 10's in ip octet
	uint8_t einer = 0;		// value of 1's in ip octet
	
	
	//
	// show ip address
	//
	
	uint8_t pos = 0; // LCD position x-axis
	
	// loop every ipv4 octet
	for(uint8_t i = 0; i < 4; i++)
	{
		hundert = 0;
		zehner = 0;
		einer = 0;
		
		octet = ip[i];
		
		// calculate 100's
		while(octet >= 100)
		{
			hundert++;
			octet -= 100;
		}
		lcd_gotoxy(1,pos++);
		lcd_putc(hundert + 48);
		
		// calculate 10's
		while(octet >= 10)
		{
			zehner++;
			octet-=10;
		}
		lcd_gotoxy(1,pos++);
		lcd_putc(zehner+48);
		
		einer = octet;
		lcd_gotoxy(1,pos++);
		lcd_putc(einer+48);
		
		// insert octet divider '.'
		if(i < 3)
		{
			lcd_gotoxy(1, pos++);
			lcd_putc('.');
		}
		
	}
	
	return;
}

/**
*
* @brief Show netmask
*
* Convert the netmask from integer to ASCII character
* and show them on the lcd.
*
* @param netmask netmask array (every ip octet have to be a array element)
*
* @return void
*
*/
void showDeviceNetmask(uint8_t* netmask)
{
	lcd_clearDisplay();
	lcd_gotoxy(0,0);
	lcd_putstr("Netzmaske:");
	
	uint8_t octet;			// copy of the current ip octet
	uint8_t hundert = 0;	// value of 100's in ip octet
	uint8_t zehner = 0;		// value of 10's in ip octet
	uint8_t einer = 0;		// value of 1's in ip octet
	
	
	//
	// display netmask
	//
	
	uint8_t pos = 0; // LCD position x-axis
	
	// loop every ipv4 octet
	for(uint8_t i = 0; i < 4; i++)
	{
		hundert = 0;
		zehner = 0;
		einer = 0;
		
		octet = netmask[i];
		
		// calculate 100's
		while(octet >= 100)
		{
			hundert++;
			octet -= 100;
		}
		lcd_gotoxy(1,pos++);
		lcd_putc(hundert + 48);
		
		// calculate 10's
		while(octet >= 10)
		{
			zehner++;
			octet-=10;
		}
		lcd_gotoxy(1,pos++);
		lcd_putc(zehner+48);
		
		einer = octet;
		lcd_gotoxy(1,pos++);
		lcd_putc(einer+48);
		
		// insert octet divider '.'
		if(i < 3)
		{
			lcd_gotoxy(1, pos++);
			lcd_putc('.');
		}
		
	}
	
	return;
}


/**
*
* @brief Show gateway ip address
*
* Convert gateway ip address from integer to ASCII character and
* display them on lcd.
*
* @param gateway gateway ip-address array (every ip octet have to be a array element)
*
* @return void
*
*/
void showDeviceGateway(uint8_t* gateway)
{
	lcd_clearDisplay();
	lcd_gotoxy(0,0);
	lcd_putstr("Gateway:");
	
	uint8_t octet;			// copy of the current ip octet
	uint8_t hundert = 0;	// value of 100's in ip octet
	uint8_t zehner = 0;		// value of 10's in ip octet
	uint8_t einer = 0;		// value of 1's in ip octet
	
	
	//
	// display gateway ip address
	//
	
	uint8_t pos = 0; // LCD position x-axis
	
	// loop every ipv4 octet
	for(uint8_t i = 0; i < 4; i++)
	{
		hundert = 0;
		zehner = 0;
		einer = 0;
		
		octet = gateway[i];
		
		// calculate 100's
		while(octet >= 100)
		{
			hundert++;
			octet -= 100;
		}
		lcd_gotoxy(1,pos++);
		lcd_putc(hundert + 48);
		
		// calculate 10's
		while(octet >= 10)
		{
			zehner++;
			octet-=10;
		}
		lcd_gotoxy(1,pos++);
		lcd_putc(zehner+48);
		
		einer = octet;
		lcd_gotoxy(1,pos++);
		lcd_putc(einer+48);
		
		// insert octet divider '.'
		if(i < 3)
		{
			lcd_gotoxy(1, pos++);
			lcd_putc('.');
		}
		
	}
	
	return;
}


/**
*
* @brief Display network configuration of the datalogger
*
* Display all network configurations on the lcd screen.
* Every parameter will be visible on screen for 2 seconds.
*
* @param ip ip-address array (every ip octet have to be a array element)
* @param netmask datalogger netmask
* @param gateway datalogger gateway ip
* @param pulse10ms pulse variable for 10ms pulse
*
* @return void
*
*/
void showNetworkConfig(uint8_t* ip, uint8_t* netmask, uint8_t* gateway, bool *pulse10ms)
{
	showDeviceIP(ip);
	delayPulseSteps(pulse10ms, 200);
	
	showDeviceNetmask(netmask);
	delayPulseSteps(pulse10ms, 200);
	
	showDeviceGateway(gateway);
	delayPulseSteps(pulse10ms, 200);
	
	return;
}