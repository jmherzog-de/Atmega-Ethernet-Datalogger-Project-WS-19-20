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
 * @file gpio.h
 * @copyright Copyright 2019 Jean-Marcel Herzog. This project is released under the MIT license.
 * @date 25.11.2019
 * @version 1
*/

#ifndef GPIO_H_
#define GPIO_H_

#include "../../ioconfig.h"
#include <stdbool.h>


#define GPIO_INPUT(DDR, PIN)	(DDR &= ~(1<<PIN))
#define GPIO_OUTPUT(DDR, PIN)	(DDR |= (1 << PIN))

#define GPIO_SET_BIT(PORT, PIN)	(PORT |= 1 << PIN)
#define GPIO_CLR_BIT(PORT, PIN) (PORT &= ~(1<<PIN))

bool GPIO_GetSwitchState(uint8_t n);

void GPIO_PrepareAsInput();
void GPIO_DeactivateAsInput();

#endif /* GPIO_H_ */