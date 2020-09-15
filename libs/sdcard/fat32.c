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
 * @file fat32.c
 * @copyright Copyright 2019 Jean-Marcel Herzog. This project is released under the MIT license.
 * @date 25.11.2019
 * @version 1
 *
 * @brief FAT32 sdcard access
 *
*/

#include "fat32.h"
#include "sdcard.h"
#include "definitions.h"
#include <string.h>

/**
*
* @brief Read SDcard boot sector
*
* @return 0: read succesfull | 1: read not succesfull
*
*/
uint8_t getBootSectorData()
{
	uint8_t res[5], token;
	
	struct BootSector_Structure *bpb;
	struct MBRInfoStructure *mbr;
	struct PartitionInfoStructure *partition;
	uint32_t dataSectors;
	
	_unusedSectors = 0;
	
	res[0] = SDReadBlock(0, &token);
	if(!(SD_R1_NO_ERROR(res[0]) && (token == 0xFE)))
	return 1;
	
	bpb = (struct BootSector_Structure *)_SDBuffer;
	
	// make sure that read sector is really boot sector
	if(bpb->jumpBoot[0] != 0xE9 && bpb->jumpBoot[0] != 0xEB)
	{
		mbr = (struct MBRInfoStructure *)_SDBuffer;
		if(mbr->signature != 0xAA55) return 1;
		
		// read first partition
		partition = (struct PartitionInfoStructure *) (mbr->partitionData);
		
		// read count of unused sectors
		_unusedSectors = partition->firstSector;
		
		// read BPB of the SDcard
		res[0] = SDReadBlock(partition->firstSector, &token);
		if(!(SD_R1_NO_ERROR(res[0]) && (token == 0xFE)))
		return 1;
		
		bpb = (struct BootSector_Structure *)_SDBuffer;
		if(bpb->jumpBoot[0] != 0xE9 && bpb->jumpBoot[0] != 0xEB) return 1;
	}
	
	_bytesPerSector = bpb->bytesPerSector;
	_sectorPerCluster = bpb->sectorPerCluster;
	_reservedSectorCount = bpb->reservedSectorCount;
	_rootCluster = bpb->rootCluster;
	_firstDataSector = bpb->hiddenSectors + _reservedSectorCount + (bpb->numberofFATs * bpb->FATsize_F32);
	
	dataSectors = bpb->totalSectors_F32 - bpb->reservedSectorCount - (bpb->numberofFATs * bpb->FATsize_F32);
	_totalClusters = dataSectors / _sectorPerCluster;
	
	return 0;
}

/**
*
* @brief Calculate sector of the cluster number
*
* @param clusterNumber cluster number where to read
*
* @return calculated cluster number
*
*/
uint32_t getFirstSector(uint32_t clusterNumber)
{
	return ((clusterNumber - 2) * _sectorPerCluster) + _firstDataSector;
}

/**
*
* @brief file name to FAT32
*
* @param fileName name of the file with file extension
*
* @return 0: conversion succesfull | 1: conversion not succesfull
*
*/
uint8_t convertFileName (char *fileName)
{
	
	unsigned char fileNameFAT[11];
	unsigned char j, k;

	for(j=0; j<12; j++)
	if(fileName[j] == '.') break;

	if(j>8) return 1;	// filename to long

	for(k=0; k<j; k++) // filename
	fileNameFAT[k] = fileName[k];

	for(k=j; k<=7; k++) // fill up end with whitespaces
	fileNameFAT[k] = ' ';

	j++;
	for(k=8; k<11; k++) // add file extension
	{
		if(fileName[j] != '\0')
		fileNameFAT[k] = fileName[j++];
		else // stop filling with whitespaces
		while(k<11)
		fileNameFAT[k++] = ' ';
	}

	for(j=0; j<11; j++) // lowercase letters to uppercase letters
	if((fileNameFAT[j] >= 0x61) && (fileNameFAT[j] <= 0x7a))
	fileNameFAT[j] -= 0x20;

	for(j=0; j<11; j++)	// copy of FAT array to input array
	fileName[j] = fileNameFAT[j];
	

	return 0;
}

/**
*
* @brief Read file
*
* This function read eall content of a file
*
* @param filename file with extension
*
* @return buffer of the content - '\0' if file not found
*
*/
char* readFile(char* filename)
{
	// read first sector
	uint32_t cluster, sector, firstSector;
	struct DirStructure *dir;
	uint8_t token;
	
	// search file
	cluster = _rootCluster;
	
	firstSector = getFirstSector(cluster);
	
	// read sector
	sector = 0;
	
	convertFileName(filename);
	SDReadBlock(firstSector + sector, &token);
	for(uint32_t i = 0; i < _bytesPerSector; i+=32)
	{
		dir = (struct DirStructure *)&_SDBuffer[i];
		if(dir->name[0] == EMPTY)
		break;
		
		if(dir->attrib == 0x20)
		{
			uint8_t j;
			for(j = 0; j < 11; j++)
			if(dir->name[j] != filename[j]) break;
			
			if(j == 11)
			{
				uint32_t file_cluster = ( (uint32_t)dir->firstClusterHI << 16 | (uint32_t)dir->firstClusterLO);
				firstSector = getFirstSector(file_cluster);
				SDReadBlock(firstSector, &token);
				char* filecontent = (char*)_SDBuffer; // make a copy because _buffer is declared as volatile
				return filecontent;
			}
		}
	}
	
	return "\0"; // no file found - return empty content
}
