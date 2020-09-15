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
 * @file fat32.h
 * @copyright Copyright 2019 Jean-Marcel Herzog. This project is released under the MIT license.
 * @date 25.11.2019
 * @version 1
*/

#ifndef FAT32_H_
#define FAT32_H_

#include <stdint.h>


/**
*
* @brief Master Boot Record structure
*
* The master boot record provides partition informations of the
* sdcard.
*
*/
struct MBRInfoStructure{
	uint8_t ignored[446];
	uint8_t partitionData[64];
	uint16_t signature;
};


/**
*
* @brief Partition structure
*
* Partition informations of the data drive.
*
*/
struct PartitionInfoStructure{
	uint8_t status;
	uint8_t headStart;
	uint16_t cylSectStart;
	uint8_t type;
	uint8_t headEnd;
	uint16_t cylSectEnd;
	uint32_t firstSector;
	uint32_t sectorsTotal;
};


/**
* @brief Boot Sector structure
*
* This structure contains all information read from boot sector.
* The boot sector can contain code for the operating system.
* At other places it contain informations about the FAT system.
*
*/
struct BootSector_Structure{
	uint8_t jumpBoot[3];			/**< jump to bootloader (EB,90 oder E9)									*/
	uint8_t OEMName[8];				/**< system name														*/
	uint16_t bytesPerSector;		/**< bytes per sector (normally 512;1024;2048 oder 4096)				*/
	uint8_t sectorPerCluster;		/**< sectors per cluster												*/
	uint16_t reservedSectorCount;	/**< count of reserved sectors (include boot sector)					*/
	uint8_t numberofFATs;			/**< count of FATs (should be 2)										*/
	uint16_t rootEntryCount;		/**< max. count of entrys in root directory								*/
	uint16_t totalSectors_F16;		/**< count of all partition sectors										*/
	uint8_t mediaType;				/**< media descriptor (0xF8 (hard drive) and 0xFF						*/
	uint16_t FATsize_F16;			/**< count of sectors per FAT (for FAT32 == 0)							*/
	uint16_t sectorsPerTrack;		/**< count of sectors per track											*/
	uint16_t numberofHeads;			/**< count of heads for BIOS INT13										*/
	uint32_t hiddenSectors;			/**< count of sectors befor partition									*/
	uint32_t totalSectors_F32;		/**< count of all sector of the partition								*/
	
	uint32_t FATsize_F32;			/**< only FAT32: count of all sectors per FAT							*/
	uint16_t extFlags;				/**< only FAT12/16: physical drive number for BIOS INT13				*/
	uint16_t FSversion;				/**< only FAT 12/16: reserved											*/
	uint32_t rootCluster;			/**< first cluster at root directory									*/
	uint16_t FSinfo;				/**< sector number of FSinfo structure									*/
	uint16_t BackupBootSector;		/**< sector number of the boot sector copy								*/
	uint8_t reserved[12];			/**< reserved															*/
	uint8_t driveNumber;			/**< physical BIOS drive number											*/
	uint8_t reserved1;				/**< reserved. Used to mark damaged file systems						*/
	uint8_t bootSignature;			/**< extended boot signature											*/
	uint32_t volumeID;				/**< filesystem-ID (serial number)										*/
	uint8_t volumeLabel[11];		/**< name of the filesystem (unused in FAT32)							*/
	uint8_t fileSystemType[8];		/**< FAT version														*/
	uint8_t bootData[420];			/**< machine code of the bootloader										*/
	uint16_t bootEndSignature;		/**< boot signature 0x55 0xAA											*/
};


/**
*
* @brief FSInfo sector data
*
* FAT32 saves extended informations at FSInfo sector.
* Normally at sector 1.
*/
struct FSInfoStructure{
	uint32_t leadSignature;			/**< FSinfo signature (0x41615252)					*/
	uint8_t reserved1[480];
	uint32_t structureSignature;	/**< extended FSinfo Signatur (0x61417272)			*/
	uint32_t freeClusterCount;		/**< count of free clusters (Initial 0xffffffff)	*/
	uint32_t nextFreeCluster;		/**< next free cluster (Initial 0xffffffff)			*/
	uint8_t reserved[12];
	uint32_t trailSignature;		/**< sector signature 0xaa550000					*/
};

/**
*
* @brief Directory structure
*
* Structur for access directory entrys at the FAT
*
*/
struct DirStructure{
	uint8_t name[11];			/**< name of the object (file/folder)	*/
	uint8_t attrib;				/**< file attribute						*/
	uint8_t NTreserved;			/**< always 0							*/
	uint8_t timeTenth;			/**< 1/10 of the length					*/
	uint16_t createTime;		/**< time at file creation				*/
	uint16_t createDate;		/**< date at file creation				*/
	uint16_t lastAccessDate;	/**< last access date					*/
	uint16_t firstClusterHI;	/**< high word of first cluster			*/
	uint16_t writeTime;			/**< time at last write access			*/
	uint16_t writeDate;			/**< date at last write access			*/
	uint16_t firstClusterLO;	/**< low word of first cluster			*/
	uint32_t fileSize;			/**< file size in bytes					*/
};

// Not implemented
struct dirLongentryStructure{
};


/**
*
* @brief Structure for file read information
*
*/
typedef struct _fileStat{
	uint32_t currentCluster;	/**< current cluster			*/
	uint32_t fileSize;			/**< file size in bytes			*/
	uint32_t currentSector;		/**< current sector				*/
	uint32_t byteCounter;		/**< count of all bytes			*/
	uint16_t sectorIndex;		/**< sector index				*/
}fileStat;

/**
*
* @brief File structure of a cluster
*
*/
typedef struct _filePosition{
	uint8_t isLongFilename;
	unsigned char *fileName;
	uint32_t startCluster;
	uint32_t cluster;
	uint32_t dirStartCluster;
	uint8_t sectorIndex;
	uint32_t sector;
	uint32_t fileSize;
	uint32_t byteCounter;
	uint16_t byte;
	uint8_t shortFilename[11];
}filePosition;


//
// Attributes for files and directorys
//
#define ATTR_READ_ONLY     0x01
#define ATTR_HIDDEN        0x02
#define ATTR_SYSTEM        0x04
#define ATTR_VOLUME_ID     0x08
#define ATTR_DIRECTORY     0x10
#define ATTR_ARCHIVE       0x20
#define ATTR_LONG_NAME     0x0F

#define DIR_ENTRY_SIZE		0x32
#define EMPTY				0x00
#define DELETED				0xE5
#define GET					0
#define SET					1
#define READ				0
#define VERIFY				1
#define ADD					0
#define REMOVE				1
#define LOW					0
#define HIGH				1
#define TOTAL_FREE			1
#define NEXT_FREE			2
#define GET_LIST			0
#define GET_FILE			1
#define DELETE				2
#define MAX_FILENAME		32


volatile uint32_t _firstDataSector;		/**< address of the first data sector				*/
volatile uint32_t _rootCluster;			/**< address of the root cluster					*/
volatile uint32_t _totalClusters;		/**< count of all clusters							*/

volatile uint16_t _bytesPerSector;		/**< count of bytes per cluster						*/
volatile uint16_t _sectorPerCluster;	/**< count of sectors in a cluster					*/
volatile uint16_t _reservedSectorCount;	/**< count of reserved sectors						*/

volatile uint32_t _unusedSectors;		/**< count of unused sectors						*/
volatile uint32_t _appendFileSector;	/**< address of next file sector					*/
volatile uint32_t _appendFileLocation;	/**< address of next file path						*/
volatile uint32_t _fileSize;			/**< file size in bytes								*/
volatile uint32_t _appendStartCluster;	/**< next start cluster								*/


// 
// Global flags for cluster and sector informations
//
uint8_t _freeClusterCountUpdated;
volatile uint32_t _fileStartCluster;


volatile uint8_t _longEntryString[MAX_FILENAME];
volatile filePosition _filePosition;



uint8_t getBootSectorData();

uint32_t getFirstSector(uint32_t clusterNumber);

uint8_t convertFileName(char* fileName);

char* readFile(char*);


#endif /* FAT32_H_ */