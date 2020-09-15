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
* @mainpage Übersicht
*
*
* @section project_description 1 Project description
* Data logger that can record analog measured values at configurable time intervals and
* transfer the measured values to a server such as e.g. thingspeak.com.
*
* @section project_hardware 2 Hardware
* The basis is the MiniMexle V2.0 with an Atmega328P. As part of the project, no proprietary
* hardware was developed, but rather existing breakout boards were used.
* 
* @subsection ethernet_controller 2.1 Ethernet SPI breakout board
* In this project, the Ethernet controller ENC28J60 from Microchip was used. There us already
* a good C library that implements all the functions required for this task. The ENC28J60
* can be connected to the MiniMexle as an SPI slave.
* 
* @subsection sd_controller 2.2 SD card breakout board
* In this project, a simple SD card module was used. It is a common module with a built in
* level converter (LVC125A). However, this causes problems beacuse the MISO pin does not
* change to a tri-state when the module is deactivated by the chip select pin.
*
* @see https://techblog.leuma.de/spi-probleme-mit-isp/
*
* @subsection analog_sensor 2.3 Simulation of analog sensor
* For the sake of simplicity, a potentiometer was connected to the ADC input of the
* microcontroller. The internal reference voltage was hereby connected to the potentiometer.
*
* @section software 3 Software Übersicht
* The software was completely implemented in C code. It can be grouped into the following main components:
*
* - libs:		libraries for controlling hardware (LCD, Ethernet, SD card)
* - views:		code for user interactions via LCD and buttons
* - routines:	procedures - provide the program logic, executed in a state of the state-machine
* - ioconfig.h:	configuration of the I/O interfaces (hardware pin configuration)
* - main.c:		main program - hardware initialization and state-machine implementation
*
* @section compile 4 Compilation
* An additional compoler flag was set in this project: <b>-Wno-deprecated-declarations</b>. This prevents 
* a deprecated warning within the tuxgraphics library for the Ethernet controller. A corresponding ToDo
* entry to fix this warning has already been created.
* Furthermore, the symbol <b>F_CPU</b> with the CPU clock frequency was defined in the project settings. 
* There is no need for a definition in source code.
*
* @section send 5 Send http request
* With this software, URL parametrerized GET requests can be transmitted to a server such as thingspeak.
* To do this, the URL must be adapted to the domain in the define WEBSERVER_URL (main.c).
* The tuxgraphics Ethernet library also includes sending POST or PUT requests. However, these are not
* currently implemented.
*
* @section see_more 5 Further information
* @see https://wiki.mexle.hs-heilbronn.de/doku.php?id=mexle:ws19_prj10_-_datenlogger 
* @see http://tuxgraphics.org/electronics/200905/embedded-tcp-ip-stack.shtml
*
* @copyright Copyright 2019 Jean-Marcel Herzog. This project is released under the MIT license
*
*/

/**
 * @author: Herzog, Jean-Marcel
 * @file main.c
 * @copyright Copyright 2019 Jean-Marcel Herzog. This project is released under the MIT license
 * @date 25.11.2019
 * @version 3
 *
 * @brief Entry point and state machine implementation.
 *
 * @todo write measurement values to sdcard, if target server is unavailable.
 * @todo edit tuxgraphics ethernet library to set user defined target URLs.
 * @todo update tuxgraphics ethernet library to prevent warning during compiling process.
 * @todo implement more http request types like PUT and POST.
 *
*/

#include <avr/io.h>

#include "ioconfig.h"

#include "libs/spi/spi.h"
#include "libs/gpio/gpio.h"
#include "libs/sdcard/sdcard.h"
#include "libs/uart/uart.h"
#include "libs/adc/adc.h"
#include "libs/ethernet/ethernet.h"
#include "libs/lcd/lcd_lib.h"


#include "views/startupView.h"
#include "views/setupView.h"
#include "views/manconfigView.h"
#include "views/idleView.h"
#include "views/showconfigView.h"
#include "views/messageView.h"
#include "views/runView.h"
#include "views/sendView.h"
#include "views/chooseSourceView.h"
#include "views/showNetworkConfig.h"


#include "routines/autoconfigRoutine.h"
#include "routines/measureRoutine.h"
#include "routines/runRoutine.h"

//
// pulse variables (pulse generated by hardware timer)
//
bool pulse10ms;		/**< pulse 10ms		*/
bool pulse500ms;	/**< pulse 500ms	*/

//
// target server configuration
//
uint8_t ip[] = {0,0,0,0};	/**< target server IPv4 Adresse	*/
uint32_t port = 0;			/**< target port				*/
uint32_t interval = 0;		/**< sending interval			*/
char hostname[25];			/**< hostname of target server	*/

//
// sensor specific variables
//
uint16_t sensorValue = 0;
uint32_t sec_until_send = 0;

//
// network parameter source
//
uint8_t selectedSource = 0;		/**< target ip or hostname 1: ip | 2: hostname */
#define SOURCE_STATICIP	2
#define SOURCE_DHCP		1


//
// state machine definitions
//
#define STATE_INIT			0	/**< initialization									*/
#define STATE_SETUP			1	/**< target parameter selection (automatic/manual)	*/
#define STATE_INITDHCP		2	/**< dhcp network init								*/
#define STATE_AUTOCONFIG	3	/**< load configuration from SDcard					*/
#define STATE_MANUALCONFIG	4	/**< configuration over display						*/
#define STATE_SHOWCONFIG	5	/**< show target parameters							*/
#define STATE_IDLE			6	/**< datalogger ready								*/
#define STATE_RUN			7	/**< datalogger running								*/
#define STATE_MEASURE		8	/**< measurement running							*/
#define STATE_SEND			9	/**< execute get request							*/
#define STATE_DNSLOOKUP		10	/**< executetarget host dns lookup					*/
#define STATE_CHOOSESOURCE	11	/**< choose target address (ip or hostname)			*/
#define STATE_MANUALIP		12	/**< manual network configuration					*/
#define STATE_SHOWNETWORK	13	/**< display network configuration					*/

uint8_t state = STATE_INIT;	/**< current datalogger state */

//
// URL
//
#define WEBSERVER_URL	"/update?api_key=H68PLC982WAV&field1="


/**
* @brief Hardware port initialization
*
* initialization of SPI hardware, Timer with overflow interrupt and ADC
*
* @return void
*/
void init_Ports(void)
{
	SPI_init();

	ADC_Init();

	initDisplay();

	// setup Timer0
	cli();
	TCCR0A = 0;
	TCCR0B = 0;
	TCNT0 = 0;
	
	OCR0A = 179;
	TCCR0A |= (1<<WGM01);
	TCCR0B |= (1<<CS02) |(1<<CS00);
	TIMSK0 |= (1<<OCIE0A);

	#ifdef DEBUG_MODE
	UART_init(BAUDRATE, F_CPU);
	#endif
	
	sei(); // activate interrupt

	return;
}





/**
*
* @brief Main loop
*
* Implementation of the state machine.
*
* @todo Bugfix send requests over target ip address given directly.
*
* @return void
*/
int main(void)
{
	init_Ports();
	
	// disable all SPI slaves
	SPI_PORT |= (1 << CS);
	SPI_PORT |= (1 << PINB2);
	
	while(1)
	{
		if(state == STATE_INIT)
		{
			startupView();
			state = STATE_INITDHCP;
		}
		/*end of if STATE_INIT*/
		
		else if(state == STATE_INITDHCP)
		{
			Ethernet_InitDHCP();
			state = STATE_SHOWNETWORK;
		}
		/*end of if STATE_INITDHCP*/
		
		else if(state == STATE_MANUALIP)
		{
			Ethernet_InitStatic();
			state = STATE_SHOWNETWORK;
		}
		/**end of if STATE_MANUALIP*/
		
		else if(state == STATE_SHOWNETWORK)
		{
			uint8_t localIP[4] = {0,0,0,0};
			uint8_t localNetmask[4] = {0,0,0,0};
			uint8_t localGateway[4] = {0,0,0,0};
			
			// DHCP Konfiguration lesen
			Ethernet_ReadNetworkConfig(localIP, localGateway, localNetmask);
			
			showNetworkConfig((uint8_t*)localIP, (uint8_t*)localNetmask, (uint8_t*)localGateway, &pulse10ms);
			
			state = STATE_SETUP;
		}
		/*end of STATE_SHOWNETWORK*/
		
		else if(state == STATE_SETUP)
		{
			switch(setupView())
			{
				case 1: state = STATE_AUTOCONFIG; break;
				case 2: state = STATE_MANUALCONFIG; break;
			}
		}
		/*end of STATE_SETUP*/
		
		else if(state == STATE_AUTOCONFIG)
		{
			if(autoconfig_routine((char*)hostname, (uint8_t*)ip, &port, &interval) == 1)
			{
				messageView("Konfig. Fehler", &pulse10ms);
				state = STATE_SETUP;
				continue;
			}
			state = STATE_SHOWCONFIG;
		}
		/*end of STATE_AUTOCONFIG*/
		
		else if(state == STATE_MANUALCONFIG)
		{
			setupIPView((uint8_t*)ip, &pulse500ms, &pulse10ms);
			setupPortView(&port, &pulse500ms, &pulse10ms);
			setupIntervalView(&interval, &pulse500ms, &pulse10ms);
			state = STATE_SHOWCONFIG;
		}
		/*end of STATE_MANUALCONFIG*/
		
		else if(state == STATE_SHOWCONFIG)
		{
			showconfigView((char*)hostname, (uint8_t*)ip, &port, &interval, &pulse10ms);
			state = STATE_CHOOSESOURCE;
		}
		/*end of STATE_SHOWCONFIG*/
		
		else if(state == STATE_IDLE)
		{
			bool idleResponse = idleView();
			
			if(idleResponse == true)
			{
				sec_until_send = interval;
				
				if(selectedSource == SOURCE_STATICIP)
				state = STATE_RUN;
				else
				state = STATE_DNSLOOKUP;
			}
			else
			{
				state = STATE_SETUP;
			}
		}
		/*end of STATE_IDLE*/
		
		else if(state == STATE_DNSLOOKUP)
		{
			messageView("> DNS Lookup", &pulse10ms);
			Ethernet_DNSLookup(hostname);
			state = STATE_RUN;
		}

		else if(state == STATE_RUN)
		{
			// stop datalogger until click on EXIT
			if(runView(&pulse10ms, &pulse500ms, &sec_until_send)){state = STATE_IDLE; continue;}
			
			if(sec_until_send == 0){state = STATE_MEASURE;}
		}
		/*end of STATE_RUN*/

		else if(state == STATE_SEND)
		{
			messageView("> senden...", &pulse10ms);
			
			// convert measure value to string for GET request
			char sensorValueString[4];
			itoa(sensorValue, sensorValueString, 10);
			
			if(selectedSource == SOURCE_STATICIP)
			Ethernet_SendGET_p(true, sensorValueString, PSTR(WEBSERVER_URL), ip, hostname);
			else
			Ethernet_SendGET_p(false, sensorValueString, PSTR(WEBSERVER_URL), NULL, hostname);
			
			sec_until_send = interval;
			
			state = STATE_RUN;
		}
		/*end of STATE_SEND*/
		
		else if(state == STATE_MEASURE)
		{
			measureRoutine(&sensorValue);
			
			state = STATE_SEND;
		}
		/*end of STATE_MEASURE*/
		
		else if(state == STATE_CHOOSESOURCE)
		{
			selectedSource = chooseSourceView();
			
			state = STATE_IDLE;
		}
		/*end of STATE_CHOOSESOURCE*/

		// reset pulse
		if(pulse10ms) pulse10ms = false;
		if(pulse500ms) pulse500ms = false;
	}
	/*end of while(1)*/

	return 0;
}

