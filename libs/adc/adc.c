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
 * @file adc.c
 * @copyright Copyright 2019 Jean-Marcel Herzog. This project is released under the MIT license.
 * @date 25.11.2019
 * @version 1
 *
 * @brief ADC library
 *
 * ADC hardware initialization and implementation of reading values in single 
 * mode and oversampling mode.
*/

#include "adc.h"

/**
 *
 * @brief ADC hardware initialization
 *
*/
void ADC_Init(void)
{
	ADMUX = (1 << REFS1) | (1 << REFS0);	// choose internal reference voltage
	
	ADCSRA = (1 << ADPS1) | (1 << ADPS0);	// ADC prescaler /8
	
	ADCSRA |= (1 << ADEN);					// activate ADC
	
	ADCSRA |= (1 << ADSC);					// single ADC conversion
	
	/* dummy readout after activation -> warump of ADC */
	
	while(ADCSRA & (1 << ADSC));
	
	(void)ADCW;								// ADC should be readen one time, either next measure value
											// wouldn't be placed.
	
	return;
}


/**
 * @brief Get analog value
 *
 * @param channel ADC channel of the uC
 *
 * @return digital measure value (0...1023)
 *
*/
uint16_t ADC_Read(uint8_t channel)
{
	ADMUX = (ADMUX & ~(0x1F)) | (channel & 0x1F);	// select ADC channel (not change other bits)
	
	ADCSRA |= (1 << ADSC);							// one conversion
	
	while(ADCSRA & (1 << ADSC));					// wait until conversion finished
	
	return ADCW;
}


/**
 *
 * @brief Get analog value with oversampling
 *
 * @param channel ADC channel of the uC.
 *
 * @param nsamples Count of measurements
 *
 * @return digital measure value (0...1023)
 *
*/
uint16_t ADC_ReadAvg(uint8_t channel, uint8_t nsamples)
{
	uint32_t sum = 0;
	
	for(uint8_t i = 0; i < nsamples; i++)
	{
		sum += ADC_Read(channel);
	}
	
	return (uint16_t) (sum / nsamples);
}