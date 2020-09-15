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
 * @file sdcard_debug.c
 * @copyright Copyright 2019 Jean-Marcel Herzog. This project is released under the MIT license.
 * @date 25.11.2019
 * @version 1
 *
 * @brief Debug functions for sdcard
 *
 * This file provide debugging functions for the SDcard interactions.
 * Response command can be transmit over UART.
 *
*/

#include "sdcard_debug.h"
#include "sdcard.h"
#include "../uart/uart.h"


/**
*
* @brief Send R1 response over UART
*
* @param res response message of format R1
*
* @return void
*
*/
void SDPrintR1(uint8_t res)
{
	if(res & 0x1000000)
	UART_puts("\tError: MSB = 1\r\n");
	if(res == 0)
	UART_puts("\tCard Ready\r\n");
	if(PARAM_ERROR(res))
	UART_puts("\tParameter Error\r\n");
	if(ADDR_ERROR(res))
	UART_puts("\tAddress Error\r\n");
	if(ERASE_SEQ_ERROR(res))
	UART_puts("\tErase Sequence Error\r\n");
	if(CRC_ERROR(res))
	UART_puts("\tCRC Error\r\n");
	if(ILLEGAL_CMD(res))
	UART_puts("\tIllegal Command\r\n");
	if(ERASE_RESET(res))
	UART_puts("\tErase Reset Error\r\n");
	if(IN_IDLE(res))
	UART_puts("\tIn Idle State\r\n");
}


/**
*
* @brief Send R7 response over UART
*
* @param res response message of type R7
*
*/
void SDPrintR7(uint8_t *res)
{
	SDPrintR1(res[0]);
	
	if(res[0] > 1) return;
	
	UART_puts("\tCommand Version: ");
	UART_puthex8(CMD_VER(res[1]));
	UART_puts("\r\n");
	
	UART_puts("\tVoltage Accepted: ");
	
	if(VOL_ACC(res[3]) == VOLTAGE_ACC_27_33)
	UART_puts("2.7-3.6V\r\n");
	else if(VOL_ACC(res[3]) == VOLTAGE_ACC_LOW)
	UART_puts("LOW VOLTAGE\r\n");
	else if(VOL_ACC(res[3]) == VOLTAGE_ACC_RES1)
	UART_puts("RESERVED\r\n");
	else if(VOL_ACC(res[3]) == VOLTAGE_ACC_RES2)
	UART_puts("RESERVED\r\n");
	else
	UART_puts("NOT DEFINED\r\n");
	
	UART_puts("\tEcho: ");
	UART_puthex8(res[4]);
	UART_puts("\r\n");
}


/**
*
* @brief Send R3 response over UART
*
* @param res response message of type R3
*
*/
void SDPrintR3(uint8_t *res)
{
	SDPrintR1(res[0]);
	
	if(res[0] > 1) return;
	
	UART_puts("\tCard Power Up Status: ");
	if(POWER_UP_STATUS(res[1]))
	{
		UART_puts("READY\r\n");
		UART_puts("\tCCS Status: ");
		if(CCS_VAL(res[1])) UART_puts("1\r\n");
		else UART_puts("0\r\n");
	}
	else
	{
		UART_puts("BUSY\r\n");
	}
	
	UART_puts("\tVDD Window: ");
	
	if(VDD_2728(res[3]))
	UART_puts("2.7-2.8, ");
	
	if(VDD_2829(res[2]))
	UART_puts("2.8-2.9, ");
	
	if(VDD_2930(res[2]))
	UART_puts("2.9-3.0, ");
	
	if(VDD_3031(res[2]))
	UART_puts("3.0-3.1, ");
	
	if(VDD_3132(res[2]))
	UART_puts("3.1-3.2, ");
	
	if(VDD_3233(res[2]))
	UART_puts("3.2-3.3, ");
	
	if(VDD_3334(res[2]))
	UART_puts("3.3-3.4, ");
	
	if(VDD_3435(res[2]))
	UART_puts("3.4-3.5, ");
	
	if(VDD_3536(res[2]))
	UART_puts("3.5-3.6");
	
	UART_puts("\r\n");
}


/**
*
* @brief Send data buffer over UART
*
* @param buffer full 512 byte databuffer
*
* @return void
*
*/
void SDPrintBuffer(uint8_t *buffer)
{
	uint8_t cols = 0;
	for(uint16_t i = 0; i < 512; i++)
	{
		UART_puthex8(*buffer++);
		if(cols == 19)
		{
			UART_puts("\r\n");
			cols = 0;
		}
		else
		{
			UART_putc(' ');
			cols++;
		}
	}
	UART_puts("\r\n");
}

/**
*
* @brief Send error token over UART
*
* @param token data token
*
* @return void
*
*/
void SDPrintDataErrorToken(uint8_t token)
{
	if(token & 0xF0)
	UART_puts("\tNot Error token\r\n");
	if(SD_TOKEN_OOR(token))
	UART_puts("\tData out of range\r\n");
	if(SD_TOKEN_CECC(token))
	UART_puts("\tCard ECC failed\r\n");
	if(SD_TOKEN_CC(token))
	UART_puts("\tCC Error\r\n");
	if(SD_TOKEN_ERROR(token))
	UART_puts("\tError\r\n");
}