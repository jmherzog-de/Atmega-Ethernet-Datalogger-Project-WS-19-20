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
 * @file showconfigView.c
 * @copyright Copyright 2019 Jean-Marcel Herzog. This project is released under the MIT license.
 * @date 25.11.2019
 * @version 1
 *
 * @brief Show datalogger parameter
 *
 * Display all datalogger configuration parameter on lcd.
 * Every parameter will be visible for 2 seconds (hostname -> ip -> port -> interval)
 *
*/

#include "showconfigView.h"
#include "../ioconfig.h"
#include "../libs/gpio/gpio.h"
#include "../libs/lcd/lcd_lib.h"
#include "../libs/delay/delay.h"

/**
*
* @brief Show ip address
*
* Convert ip address from integer to ASCII character
* and show them on the lcd.
*
* @param ip ip-address array (every octet have to be a array element)
*
* @return void
*
*/
void showIP(uint8_t *ip)
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
* @brief Show port or interval on the lcd
*
* convert a 32 bit integer to ASCII characters
*
* @param display 0: display port | 1: display interval
*
* @param data value to be shown
*
* @return void
*
*/
void showPortInterval(uint8_t display, uint32_t *data)
{
	lcd_clearDisplay();
	lcd_gotoxy(0,0);
	if(display == 0)
	lcd_putstr("Port:");
	else
	lcd_putstr("Interval");
	
	uint8_t tausend = 0;
	uint8_t hundert = 0;
	uint8_t zehner = 0;
	
	uint32_t p = *data;
	
	while(p >= 1000)
	{
		p-=1000;
		tausend++;
	}
	lcd_gotoxy(1,0);
	lcd_putc(tausend+48);
	
	while(p>=100)
	{
		p-=100;
		hundert++;
	}
	lcd_gotoxy(1,1);
	lcd_putc(hundert+48);
	
	while(p>=10)
	{
		p-=10;
		zehner++;
	}
	lcd_gotoxy(1,2);
	lcd_putc(zehner+48);
	
	lcd_gotoxy(1,3);
	lcd_putc(p+48);
	
	return;
}

/**
*
* @brief Show hostname
*
* @param data hostname
*
* @return void
*
*/
void showHostname(char* data)
{
	
	lcd_clearDisplay();
	lcd_gotoxy(0,0);
	lcd_putstr("Host:");
	
	uint8_t pos = 0;
	while(data[pos] != 0)
	{
		lcd_gotoxy(1, pos);
		lcd_putc(data[pos]);
		pos++;
	}
	
	return;
}

/**
*
* @brief Show configuration parameters
*
* Show every parameter for 2 seconds.
*
* @param ip array of ip address
* @param port Pointer for port
* @param interval Pointer for interval
* @param pulse10ms pulse with a durations of 10ms
*
* @return void
*
*/
void showconfigView(char* hostname, uint8_t *ip, uint32_t *port, uint32_t *interval, bool *pulse10ms)
{
	
	showHostname(hostname);
	delayPulseSteps(pulse10ms, 200);
	
	showIP(ip);
	delayPulseSteps(pulse10ms, 200);
	
	showPortInterval(0, port);
	delayPulseSteps(pulse10ms, 200);
	
	showPortInterval(1, interval);
	delayPulseSteps(pulse10ms, 200);
	
	return;
}