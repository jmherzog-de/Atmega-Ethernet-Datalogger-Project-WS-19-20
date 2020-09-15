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
 * @file ethernet.h
 * @copyright Copyright 2019 Jean-Marcel Herzog. This project is released under the MIT license.
 * @date 25.11.2019
 * @version 1
*/

#ifndef ETHERNET_H_
#define ETHERNET_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <stdbool.h>

void Ethernet_InitDHCP();

void Ethernet_InitStatic();

void Ethernet_ReadNetworkConfig(uint8_t* ip, uint8_t* gateway, uint8_t* mask);

void Ethernet_SendGET_p(bool useIP, char* value, const char* requestUrl, uint8_t* ip, const char* host);

void Ethernet_DNSLookup(const char* host);

#endif /* ETHERNET_H_ */