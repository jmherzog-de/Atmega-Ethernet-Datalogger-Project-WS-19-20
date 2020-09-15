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
 * @file sdcard.h
 * @copyright Copyright 2019 Jean-Marcel Herzog. This project is released under the MIT license.
 * @date 25.11.2019
 * @version 1
*/

#ifndef SDCARD_H_
#define SDCARD_H_

#define SD_SUCCESS			0
#define SD_FAIL				1

#include "fat32.h"

#include <stdint.h>

volatile uint8_t _SDBuffer[512];	/**< Global data buffer for read data from SDcard */


void SDPowerUpSequence(void);

void SDCommand(uint8_t, uint32_t, uint8_t);

uint8_t SDReadR1(void);

void SDReadR7(uint8_t*);

void SDSendIfCond(uint8_t*);

void SDReadOCR(uint8_t*);

uint8_t SDSendApp(void);

uint8_t SDSendOpCond(void);

uint8_t SDReadBlock(uint32_t, uint8_t*);

uint8_t SDGoIdleState(void);

uint8_t SDInit(void);

#endif /* SDCARD_H_ */