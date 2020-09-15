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
 * @file autoconfigRoutine.c
 * @copyright Copyright 2019 Jean-Marcel Herzog. This project is released under the MIT license.
 * @date 25.11.2019
 * @version 1
 *
 * @brief Automatic configuration over SDcard
 *
 * This configuration routine establish connection to SDcard and read the configuration
 * parameters for the datalogger at file 'config.txt'.
 * Parameters have to be in follow format: IP;Port;Interval;Hostname;
 *
 * @note for this file, the SD library is required. Please include sdcard.h
 *
*/


#include "autoconfigRoutine.h"
#include "../libs/sdcard/sdcard.h"
#include <string.h>

/**
*
* @brief Configuration structure
*
*/
struct Config_Structure{
	uint8_t ip_address[4];
	uint32_t port;
	uint32_t interval;
	char host[25];
};

/**
*
* @brief Extract ip from input string
*
* @param posIndex position index in file
* @param config configuration structure
* @param data string out of textfile
* @return 1: no valid ip found | 2: invalid character found | 0: OK
*
*/
uint8_t extractIPAddress(uint8_t* posIndex, struct Config_Structure *config, char* data)
{
	uint8_t octet = 0;				// ip octet
	uint8_t n = 0;					// position variable in octet
	uint8_t tmp_ip[3] = {0,0,0};	// temp ip octet
	
	// check if ip is in configuration file
	if(data[*posIndex] == ';')
	{
		return 1;
	}
	
	while(octet < 4)
	{

		// check if current character is valid
		if( (data[*posIndex] < '0' || data[*posIndex] > '9') && data[*posIndex] != '.' && data[*posIndex] != ';')
		{
			return 2;
		}
		
		// end of ip oketet or end of ip address detected
		if(data[*posIndex] == '.' || data[*posIndex] == ';')
		{
			if(n == 3)
			config->ip_address[octet] = tmp_ip[0] * 100 + tmp_ip[1] * 10 + tmp_ip[2];
			else if(n == 2)
			config->ip_address[octet] = tmp_ip[0] * 10 + tmp_ip[1];
			else
			config->ip_address[octet] = tmp_ip[0];
			
			// reset position variable in octet
			n = 0;
			tmp_ip[0] = 0;
			tmp_ip[1] = 0;
			tmp_ip[2] = 0;
			
			// ipv4 end reached? 
			if(data[*posIndex] == ';') {*posIndex += 1; break;}
			
			octet++;
			*posIndex += 1;
		}
		
		// convert ASCII character to number
		tmp_ip[n++] = (uint8_t) (data[*posIndex] - 48);
		*posIndex += 1;
	}
	
	return 0;
}

/**
*
* @brief Extract port from input string
*
* @param posIndex position index in textfile
* @param config configuration structure
* @param data string out of textfile
* @return 1: invalid character detected | 0: OK
*
*/
uint8_t extractPort(uint8_t* posIndex, struct Config_Structure *config, char* data)
{
	uint8_t portVar[4] = {0,0,0,0};
	uint8_t pos = 0;
	while(data[*posIndex] != ';' && pos < 4)
	{
		// valid character?
		if(data[*posIndex] < '0' || data[*posIndex] > '9')
		return 1;
		
		portVar[pos++] = (uint8_t) (data[*posIndex] - 48);
		*posIndex +=1 ;
	}
	
	*posIndex += 1; // ';' character overjump
	
	// build port value
	if(pos == 4)
	config->port = portVar[0] * 1000 + portVar[1] * 100 + portVar[2] * 10 + portVar[3];
	else if(pos == 3)
	config->port = portVar[0] * 100 + portVar[1] * 10 + portVar[2];
	else if(pos == 2)
	config->port = portVar[0] * 10 + portVar[2];
	else
	config->port = portVar[0];
	
	return 0;
}

/**
*
* @brief Extract interval from input string
*
* @param posIndex position index in textfile
* @param config configuration structure
* @param data string out of textfile
* @return 1: invalid character detected | 0: OK
*/
uint8_t extractInterval(uint8_t *posIndex, struct Config_Structure *config, char* data)
{
	uint8_t intervalVar[4] = {0,0,0,0};
	uint8_t pos = 0;
	while(data[*posIndex] != ';' && pos < 4)
	{
		// invalid character?
		if(data[*posIndex] < '0' || data[*posIndex] > '9')
		return 1;
		
		intervalVar[pos++] = (uint8_t) (data[*posIndex] - 48);
		*posIndex += 1;
	}
	
	*posIndex += 1; // ';' overjump character
	
	// build inveral value
	if(pos == 4)
	config->interval = intervalVar[0] * 1000 + intervalVar[1] * 100 + intervalVar[2] * 10 + intervalVar[3];
	else if(pos == 3)
	config->interval = intervalVar[0] * 100 + intervalVar[1] * 10 + intervalVar[2];
	else if(pos == 2)
	config->interval = intervalVar[0] * 10 + intervalVar[2];
	else
	config->interval = intervalVar[0];
	
	return 0;
}


/**
*
* @brief Extract hostname from input string
*
* @param posIndex position index in textfile
* @param config configuration structure
* @param data string out of textfile
* @return void
*
*/
void extractHostname(uint8_t *posIndex, struct Config_Structure *config, char* data)
{
	uint8_t i = 0; // host string positon index
	while(data[*posIndex] != ';')
	{
		config->host[i++] = data[*posIndex];
		*posIndex += 1;
		
	}
	config->host[i] = '\0';
	
	return;
}


/**
*
* @brief Automatic configuration routine
*
* Load all parameter from config.txt
*
* @param host hostname Pointer
* @param ip_address ip address pointer
* @param port port Pointer
* @param interval interval Pointer
*
* @return 1: file empty or not found | 0: routine sucessfull
*
*/
uint8_t autoconfig_routine(char* host, uint8_t *ip_address, uint32_t *port, uint32_t *interval)
{
	
	// initialize SDcard
	if(SDInit() == SD_FAIL) return 1;
	

	// read boot sector
	if(getBootSectorData())
	return 1;
	
	// read configuration file
	char* data = readFile("config.txt");
	
	// file available and not empty?
	if(data == '\0')
	return 1;
	
	// extract data
	struct Config_Structure config;
	
	uint8_t posIndex = 0; // string position index
	
	if(extractIPAddress(&posIndex, &config, data) == 1)
	return 1;
	
	if(extractPort(&posIndex, &config, data) == 1)
	return 1;
	
	if(extractInterval(&posIndex, &config, data) == 1)
	return 1;
	
	extractHostname(&posIndex, &config, data);
	
	
	memcpy(host, config.host, 25);
	ip_address[0] = config.ip_address[0];
	ip_address[1] = config.ip_address[1];
	ip_address[2] = config.ip_address[2];
	ip_address[3] = config.ip_address[3];
	*interval = config.interval;
	*port = config.port;
	
	return 0;
}