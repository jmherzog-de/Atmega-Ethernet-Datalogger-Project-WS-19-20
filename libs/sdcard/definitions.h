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
 * @file definitions.h
 * @copyright Copyright 2019 Jean-Marcel Herzog. This project is released under the MIT license.
 * @date 25.11.2019
 * @version 1
*/

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#include "../../ioconfig.h"

#define CS_ENABLE			SPI_PORT &= ~(1<<CS)	/**< activate SD module */
#define CS_DISABLE			SPI_PORT |= (1<<CS)		/**< deactivate SD module */


#define CMD0                0
#define CMD0_ARG            0x00000000
#define CMD0_CRC            0x95


#define CMD8                8
#define CMD8_ARG            0x0000001AA
#define CMD8_CRC            0x87


#define CMD55               55
#define CMD55_ARG           0x00000000
#define CMD55_CRC           0x00


#define CMD58               58
#define CMD58_ARG           0x00000000
#define CMD58_CRC           0x00


#define ACMD41              41
#define ACMD41_ARG          0x40000000
#define ACMD41_CRC          0x00


#define CMD17				17
#define CMD17_CRC			0x00


#define SD_READY				0x00
#define SD_IN_IDLE_STATE		0x01
#define SD_START_TOKEN          0xFE
#define CMD0_MAX_ATTEMPTS       255
#define CMD55_MAX_ATTEMPTS      255
#define SD_R1_NO_ERROR(x)		x < 0x02
#define SD_MAX_READ_ATTEMPTS	1563


#endif /* DEFINITIONS_H_ */