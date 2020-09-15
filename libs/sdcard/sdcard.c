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
 * @file sdcard.c
 * @copyright Copyright 2019 Jean-Marcel Herzog. This project is released under the MIT license.
 * @date 25.11.2019
 * @version 1
 *
 * @brief Access to SDcard
 *
 * Implementation of SDcard intertactions over SPI
 *
*/

#include "sdcard.h"
#include "sdcard_debug.h"
#include "../uart/uart.h"
#include "../spi/spi.h"
#include "definitions.h"


/**
*
* @brief Start initialization sequence
*
* SDcard need a minimum of 1ms and 74 clock cycles (SPI) befor
* commands can be send. Therefore, this function implement a delay of 1ms and send
* 10 bytes over SPI (1 byte = 8 clock cylces). Chip Select have to be deselected.
*
* @note for more informations see the documentation of the SDcard Foundation at picture 6-4
*
* @return void
*
*/
void SDPowerUpSequence(void)
{
	CS_DISABLE;
	
	_delay_ms(1);
	
	for(uint8_t i = 0; i < 0x0A; i++)
	SPI_transreceive(0xFF);
}


/**
*
* @brief Send command to SDcard
*
* All commands are 6 byte long and contain a command index, userdefined 
* data and ending with a cyclic redundancy check (CRC).
* If no arguments required -> send 0x00
*
* @param cmd command index
* @param arg userdefined data
* @param crc Cyclic Redundancy Check byte
*
* @note for mre informations see the documentation of the SDcard Foundation at picture 7-1
*
* @return void
*/
void SDCommand(uint8_t cmd, uint32_t arg, uint8_t crc)
{
	// send command to SDcard
	// bit number 47 have to be 1 (bitwise or with 0x40)
	SPI_transreceive(cmd | 0x40);
	
	// send userdefined data
	SPI_transreceive( (uint8_t) (arg >> 24) );
	SPI_transreceive( (uint8_t) (arg >> 16) );
	SPI_transreceive( (uint8_t) (arg >> 8) );
	SPI_transreceive( (uint8_t) arg );
	
	// transmit CRC
	SPI_transreceive(crc | 0x01);
}


/**
*
* @brief Read R1 message
*
* Read a 1 byte response from SDcard. Bit number 7 is always 0.
* All other bits representing a error.
*
* @note for more informations see the documentation of the SDcard Foundation at picture 7-19
*
* @return Response message of type R1
*/
uint8_t SDReadR1(void)
{
	uint8_t noDataLoop = 0;	// counter variable for polling loop
	uint8_t response = 0;	// received byte
	
	// polling until data is reveived
	while( (response = SPI_transreceive(0xFF)) == 0xFF )
	{
		noDataLoop++;
		
		// no data for 8 bytes?
		if(noDataLoop > 8) break;
	}
	
	return response;
}


/**
*
* @brief Read response format R7
*
* The R7 format is 8 byte long. First byte is identical to R1 response.
* Followed by a command version field, voltage accepted field and a Echo-back CRC field of
* the transmitted command to SDcard.
* SDcards of first generation, only R1 section will be filled with data.
* Every other section will be 0.
* @param res Pointer für gelesenen Inhalt
*
* @note for more informations see the documentation of the SDcard Foundation at picture 7-12
*
* @return void
*/
void SDReadR7(uint8_t* res)
{
	res[0] = SDReadR1();
	
	// Check error bit (min. one bit between bit 1 and 7 set?)
	if(res[0] > 1) return;
	
	// read the other bytes (send dummy bytes 0xFF - don't care)
	res[1] = SPI_transreceive(0xFF);
	res[2] = SPI_transreceive(0xFF);
	res[3] = SPI_transreceive(0xFF);
	res[4] = SPI_transreceive(0xFF);
}


/**
*
* @brief Send Interface Condition Command (CMD8)
*
* Command check if SDcard generation is 1 or higher. If SDCard of gen. 1 is detected
* bit 2 in R1 will be set.
*
* @param res received data pointer
*
* @todo Implementation of a SDcard version detection
*
* @note Only SDcards of second generation and higher are working with this project. A version control will not be done at this code.
*
* @note for more informatins see the documentation oof the SDcard Foundation at table 4-18
*
* @return void
*
*/
void SDSendIfCond(uint8_t *res)
{
	// activate SDcard over chip select
	SPI_transreceive(0xFF);
	CS_ENABLE;
	SPI_transreceive(0xFF);
	
	// send Interface Condition Command
	SDCommand(CMD8, CMD8_ARG, CMD8_CRC);
	
	// read response
	SDReadR7(res);
	
	// deactivate SDcard over chip select
	SPI_transreceive(0xFF);
	CS_DISABLE;
	SPI_transreceive(0xFF);
}


/**
*
* @brief First Sending Command
*
* Command to SDcard. Next command is a application-specific command and contains
* no transfer data (like a standard command)
*
* @return R1 Response Nachricht
*
*/
uint8_t SDSendApp(void)
{
	uint8_t response = 0;
	
	// activate SDcard over chip select
	SPI_transreceive(0xFF);
	CS_ENABLE;
	SPI_transreceive(0xFF);
	
	// send First Sending Command (important: CRC have to be right, no dummy send!)
	SDCommand(CMD55, CMD55_ARG, CMD55_CRC);
	
	// read R1 response message
	response = SDReadR1();
	
	// deactivate SDcard over chip select
	SPI_transreceive(0xFF);
	CS_DISABLE;
	SPI_transreceive(0xFF);
	
	return response;
}


/**
*
* @brief Send Operating Condition Command (ACMD41)
*
* This function send the host capacity supper information and starts the
* card initialization.

* @return R1 response message
*
*/
uint8_t SDSendOpCond(void)
{
	uint8_t res;
	
	// activate SDcard over chip select
	SPI_transreceive(0xFF);
	CS_ENABLE;
	SPI_transreceive(0xFF);
	
	// send Capacity Conditon Command (important: CRC have to be right, no dummy send!)
	SDCommand(ACMD41, ACMD41_ARG, ACMD41_CRC);
	
	// read R1 response
	res = SDReadR1();
	
	// deactivate SDcard over chip select
	SPI_transreceive(0xFF);
	CS_DISABLE;
	SPI_transreceive(0xFF);
	
	return res;
}


/**
*
* @brief Read Single Block (CMD17)
*
* Read a single datablock from a specific address.
*
* @note SDHC und SDXC Karten have block-addresses, this mens the address 0 return values
* from byte 0-511 or address 1 return values from address 512-1023. The values where
* stored at _buffer[] (sdcard.h).
*
* @param addr address/sector of the datablock
*
* @param token Token of the read datablock
*
* @return R1 response message
*
*/
uint8_t SDReadBlock(uint32_t addr, uint8_t *token)
{
	uint8_t res, read;
	uint16_t attempts;
	
	// empty data token
	*token = 0xFF;
	
	// activate SDcard over chip select
	SPI_transreceive(0xFF);
	CS_ENABLE;
	SPI_transreceive(0xFF);
	
	// send Read Single Block Command (CMD17)
	SDCommand(CMD17, addr, CMD17_CRC);
	
	// read response of format R1
	res = SDReadR1();
	
	// check if values are sent back
	if(res != 0xFF)
	{
		// wait for response token
		attempts = 0;
		while(++attempts != SD_MAX_READ_ATTEMPTS)
		if( (read = SPI_transreceive(0xFF)) != 0xFF ) break;
		
		if(read == SD_START_TOKEN)
		{
			// read 512 byte datablock
			for(uint16_t i = 0; i < 512; i++)
			_SDBuffer[i] = SPI_transreceive(0xFF);
			
			// read CRC
			SPI_transreceive(0xFF);
			SPI_transreceive(0xFF);
		}
		
		// fill up token with SD response
		*token = read;
	}
	
	// deactivate SDcard over chip select
	SPI_transreceive(0xFF);
	CS_DISABLE;
	SPI_transreceive(0xFF);
	
	return res;
}


/**
*
* @brief Read Operation Conditions Register (OCR) CMD58
*
* Read the OCR register. CSS bit is allocated to the OCR register.
* This command return a response message of format R3. This response
* contain all available operating voltages of the SDcard.
*
* @note for more informatins see the documentation oof the SDcard Foundation at table 5-1
*
* @param res Pointer for R3 response message
*
* @return void
*
*/
void SDReadOCR(uint8_t *res)
{
	// activate SDcard over chip select
	SPI_transreceive(0xFF);
	CS_ENABLE;
	SPI_transreceive(0xFF);
	
	SDCommand(CMD58, CMD58_ARG, CMD58_CRC);
	
	// Response R3 has same length as R7
	// therefore it can be used the same function for reading
	SDReadR7(res);
	
	// deactivate SDcard over chip select
	SPI_transreceive(0xFF);
	CS_DISABLE;
	SPI_transreceive(0xFF);
}


/**
*
* @brief Set SDcard to idle state
*
* Send a init command (CMD0) to bring SDcard to idle mode.
*
* @return R1 response format
*
*/
uint8_t SDGoIdleState(void)
{
	uint8_t response;
	
	// activate SDcard over chip select
	SPI_transreceive(0xFF);
	CS_ENABLE;
	SPI_transreceive(0xFF);
	
	SDCommand(CMD0, CMD0_ARG, CMD0_CRC);
	
	response = SDReadR1();
	
	// deactivate SDcard over chip select
	SPI_transreceive(0xFF);
	CS_DISABLE;
	SPI_transreceive(0xFF);
	
	return response;
}


/**
*
* @brief SDcard initialization
*
* full initialization porcess after picture 7-12 of the SDcard Foundation documentation.
*
* @note no implementation for SDcard of generation 1.x
*
* @return 0: initialization error | 1: initialization sucessfull
*
*/
uint8_t SDInit(void)
{
	uint8_t res[5], attempts = 0;
	
	SDPowerUpSequence();
	
	// command SDcard to idle mode (CMD0)
	while( (res[0] = SDGoIdleState()) != SD_IN_IDLE_STATE )
	{
		attempts++;
		
		if(attempts > 10)
		return SD_FAIL;
	}
	
	
	_delay_ms(1);
	
	// send Interface Condition (CMD8)
	SDSendIfCond(res);
	
	
	// check response and echo pattern
	if(res[0] != 0x01 || res[4] != 0xAA)
	return SD_FAIL;
	
	
	// attempt counter for SDcard initialization
	attempts = 0;
	
	do{
		
		if(attempts == CMD55_MAX_ATTEMPTS)
		return SD_FAIL;
		
		res[0] = SDSendApp();
		if(SD_R1_NO_ERROR(res[0]))
		res[0] = SDSendOpCond();
		
		_delay_ms(1);
		attempts++;
		
	}while(res[0] != SD_READY);
	
	_delay_ms(1); // SDcard need a short moment before OCR register can be read.
	
	SDReadOCR(res);
	
	return SD_SUCCESS;
}

