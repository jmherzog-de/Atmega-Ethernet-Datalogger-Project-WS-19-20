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
 * @file sdcard_debug.h
 * @copyright Copyright 2019 Jean-Marcel Herzog. This project is released under the MIT license.
 * @date 25.11.2019
 * @version 1
*/

#ifndef SDCARD_DEBUG_H_
#define SDCARD_DEBUG_H_

#include "../uart/uart.h"
#include "../spi/spi.h"


#define SD_TOKEN_OOR(X)     X & 0b00001000
#define SD_TOKEN_CECC(X)    X & 0b00000100
#define SD_TOKEN_CC(X)      X & 0b00000010
#define SD_TOKEN_ERROR(X)   X & 0b00000001
#define CMD_VER(x)			((x >> 4) & 0xF0)
#define VOL_ACC(x)			(x & 0x1F)
#define VOLTAGE_ACC_27_33	0b00000001
#define VOLTAGE_ACC_LOW		0b00000010
#define VOLTAGE_ACC_RES1	0b00000100
#define VOLTAGE_ACC_RES2	0b00001000
#define PARAM_ERROR(x)		x & 0b01000000
#define ADDR_ERROR(x)		x & 0b00100000
#define ERASE_SEQ_ERROR(x)	x & 0b00010000
#define CRC_ERROR(x)		x & 0b00001000
#define ILLEGAL_CMD(x)		x & 0b00000100
#define ERASE_RESET(x)		x & 0b00000010
#define IN_IDLE(x)			x & 0b00000001
#define POWER_UP_STATUS(X)  X & 0x40
#define CCS_VAL(X)          X & 0x40
#define VDD_2728(X)         X & 0b10000000
#define VDD_2829(X)         X & 0b00000001
#define VDD_2930(X)         X & 0b00000010
#define VDD_3031(X)         X & 0b00000100
#define VDD_3132(X)         X & 0b00001000
#define VDD_3233(X)         X & 0b00010000
#define VDD_3334(X)         X & 0b00100000
#define VDD_3435(X)         X & 0b01000000
#define VDD_3536(X)         X & 0b10000000

void SDPrintR1(uint8_t);

void SDPrintR7(uint8_t*);

void SDPrintR3(uint8_t*);

void SDPrintBuffer(uint8_t*);

void SDPrintDataErrorToken(uint8_t);




#endif /* SDCARD_DEBUG_H_ */