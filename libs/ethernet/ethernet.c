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
 * @file ethernet.c
 * @copyright Copyright 2019 Jean-Marcel Herzog. This project is released under the MIT license.
 * @date 25.11.2019
 * @version 1
 *
 * @brief ENC28J60 Ethernet library
 *
 * Library to communicate with the ENC28J60 Ethernet Controller over SPI.
 *
 * @note hardware specific configuration is placed in tuxgraphics/enc28j60.h
 *
*/

#include "ethernet.h"
#include <string.h>

#include "tuxgraphics/ip_arp_udp_tcp.h"
#include "tuxgraphics/websrv_help_functions.h"
#include "tuxgraphics/enc28j60.h"
#include "tuxgraphics/timeout.h"
#include "tuxgraphics/net.h"
#include "tuxgraphics/dnslkup.h"
#include "tuxgraphics/dhcp_client.h"

static uint8_t deviceMac[6] = { 0x6A, 0x77, 0x6A, 0x10, 0x00, 0x29};	/**< MAC address of the datalogger (unique in network) */

static uint8_t deviceIP[4] = {192, 168, 130, 20};						/**< locale IP (configured over dhcp) */

static uint8_t deviceGw[4] = {192, 168, 130, 1};						/**< Gateway IP (configured over dhcp) */

static uint8_t netmask[4] = {255, 255, 255, 0};							/**< Netmask of the local network (configured over dhcp) */
	
#define TRANS_NUM_GWMAC 1	/**< Transaction number for dhcp gateway MAC */
static uint8_t gwmac[6];	/**< Gateway MAC address*/

static uint8_t destIP[4]; /**< Target IP */

/**
*
* @brief DNS Lookup state
*
* States for the DNS lookup
*
*/
typedef enum
{
	dnsStateIdle,
	dnsStateRequestSent,
	dnsStateHaveAnswer	
}dnsStateEnum;

static dnsStateEnum dns_state = dnsStateIdle; /**< Current DNS state */


#define BUFFER_SIZE	650					/**< Recv/Transmit Buffer size	*/
static uint8_t buf[BUFFER_SIZE+1];		/**< Buffer storage				*/
static uint8_t startWebClient = 0;		/**< Web Client status			*/
static uint8_t gwArpState = 0;			/**< Gateway detection status	*/

/**
*
* @brief Ping Callback function
*
* This function get called, when a ping is received.
*
* @param ip IP of the ping sender
*
* @return void
*
*/
void pingCallback(uint8_t *ip)
{
	return;
}


/**
*
* @brief HTTP Request Callback
*
* This function get called, when a response to a http request is available.
*
* Callback set the http status code to 200. The web client set to wait mode to
* be able to send a new request.
*
* @note die __attribute__((unused)) is a GCC Compiler directive to avoid warning while compiling.
*
* @param webStatusCode First number of http status code eg. (200 - 2 |502 - 5)
*
* @return void
*
*/
void browserresultCallback(uint8_t webStatusCode,uint16_t datapos __attribute__((unused)), uint16_t len __attribute__((unused)))
{
	if(webStatusCode == 2)
	{
		startWebClient = 3;	// waiting mode
	}
	else
	{
		startWebClient = 0; // Stopped
	}
}


/**
*
* @brief ARP Request Callback function
*
* This function get called, whe a arp request is reveived.
* Received mac address will be copied to gateway mac-address.
*
* @note die __attribute__((unused)) is a GCC Compiler directive to avoid warning while compiling.
*
* @param transactionNumber transaction number of the arp request.
* @param mac MAC address of the gateway
*
* @return void
*
*/
void arpresolverResultCallback(uint8_t *ip __attribute__((unused)),uint8_t transactionNumber,uint8_t *mac)
{
	if (transactionNumber==TRANS_NUM_GWMAC)
	memcpy( gwmac, mac, 6 );
}

/**
*
* @brief DHCP initialization
*
* @return void
*
*/
void Ethernet_InitDHCP()
{
	uint16_t plen;
	
	enc28j60Init(deviceMac);			// Ethernet Controller init

	_delay_us(5);
	
	enc28j60PhyWrite(PHLCON, 0x476);	// activate LED (Rx blink)
	
	while(enc28j60linkup() == 0);		// make sure, controller is initialized
	
	init_mac(deviceMac);				// send mac address to ethernet controller
	

	// loop until configuration done
	uint8_t i = 0;
	while(i == 0)
	{
		plen = enc28j60PacketReceive(BUFFER_SIZE, buf);
		buf[BUFFER_SIZE] = '\0';
		i = packetloop_dhcp_initial_ip_assignment(buf, plen, deviceMac[5]);
	}
	
	dhcp_get_my_ip(deviceIP, netmask, deviceGw);	// read ip address from device
	
	client_ifconfig(deviceIP, netmask);				// transmit received ip to ethernet controller
	
	return;
}

/**
*
* @brief Read network parameter
*
* @param ip ip-address of the datalogger
* @param gateway gateway address of the network
* @param mask netmask of the network
* @return void
*
*/
void Ethernet_ReadNetworkConfig(uint8_t* ip, uint8_t* gateway, uint8_t* mask)
{
	ip[0] = deviceIP[0];
	ip[1] = deviceIP[1];
	ip[2] = deviceIP[2];
	ip[3] = deviceIP[3];
	
	gateway[0] = deviceGw[0];
	gateway[1] = deviceGw[1];
	gateway[2] = deviceGw[2];
	gateway[3] = deviceGw[3];
	
	mask[0] = netmask[0];
	mask[1] = netmask[1];
	mask[2] = netmask[2];
	mask[3] = netmask[3];
	
	return;
}

/**
*
* @brief Set target ip address
*
* @param ip target ip
* @return void
*
*/
void Ethernet_SetDestIP(uint8_t* ip)
{
	destIP[0] = ip[0];
	destIP[1] = ip[1];
	destIP[2] = ip[2];
	destIP[3] = ip[3];
	
	return;
}

/**
*
* @brief Manual network configuration
*
* @return void
*
*/
void Ethernet_InitStatic()
{
	enc28j60Init(deviceMac);
	_delay_us(5);
	enc28j60PhyWrite(PHLCON, 0x476);
	while(enc28j60linkup() == 0);
	client_ifconfig(deviceIP, NULL);
	init_mac(deviceMac);
	get_mac_with_arp(deviceGw, TRANS_NUM_GWMAC, &arpresolverResultCallback);
	get_mac_with_arp_wait();
	return;
}

/**
*s
* @brief Send GET request
*
* @param value value to send
* @param request_url url of the request (without hostname)
* @param host hostname (eg. api.thingspeak.com)
*
* @return void
*
*/
void Ethernet_SendGET_p(bool useIP, char* value, const char* requestUrl, uint8_t* ip, const char* host)
{
	uint16_t plen, dat_p;

	uint8_t endTransfer = 0;
	while(!endTransfer)
	{
		plen = enc28j60PacketReceive(BUFFER_SIZE, buf);		// read packet buffer
		dat_p = packetloop_arp_icmp_tcp(buf, plen);
		
		// data available?	- if data availble, start to stransmit
		if(plen == 0)
		{
			if(startWebClient == 1)
			{
				startWebClient = 2;
				client_browse_url(requestUrl, value, host, &browserresultCallback, destIP, gwmac);
			}
			
			// response available?
			if(startWebClient == 3)
			{
				startWebClient = 1; // ready to send a new request
				endTransfer = 1;
			}
		}
		
		if(dat_p == 0)
		{
			// process incomming messages
			// needed to make ping requests
			udp_client_check_for_dns_answer(buf, plen);
			continue;
		}
	}
	
	return;
}

/**
* @brief DNS lookup function
*
* @param host hostname (eg. api.thingspeak.com)
*
* @return void
*/
void Ethernet_DNSLookup(const char* host)
{
	uint16_t dat_p, plen;
	
	uint8_t dns_success = 0;
	while(!dns_success)
	{
		plen = enc28j60PacketReceive(BUFFER_SIZE, buf);		// read packet buffer
		dat_p = packetloop_arp_icmp_tcp(buf, plen);			// receive ping
		
		// packets available?
		if(plen == 0)
		{
			// Process Arp and DNS
			if(gwArpState == 0)
			{
				// Lookup mac address of the gateway
				get_mac_with_arp(deviceGw, TRANS_NUM_GWMAC, &arpresolverResultCallback);
				gwArpState = 1;
			}
			
			if(get_mac_with_arp_wait() == 0 && gwArpState == 1)
			{
				// Gateway mac address available
				gwArpState = 2;
			}
			
			if(dns_state == dnsStateIdle && gwArpState == 2)
			{
				// network connection etablished?
				if(!enc28j60linkup())
				continue;
				
				dns_state = dnsStateRequestSent;
				dnslkup_request(buf, host, gwmac); // target host dns lookup
				continue;
			}
			
			if(dns_state == dnsStateRequestSent && dnslkup_haveanswer())
			{
				dns_state = dnsStateHaveAnswer;
				dnslkup_get_ip(destIP);
				startWebClient = 1; // controller ready to send
				dns_success = 1;	// dns lookup successfull
			}
			
			if(dns_state != dnsStateHaveAnswer)
			{
				// retry after dns lookup failed
				_delay_ms(2000);
				continue;
			}
		}
		
		if(dat_p == 0)
		{
			// process incomming messages
			// needed for ping requests to ethernet controller
			udp_client_check_for_dns_answer(buf, plen);
			continue;
		}
	}
}