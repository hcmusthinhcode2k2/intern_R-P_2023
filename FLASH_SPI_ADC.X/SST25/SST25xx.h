#ifndef SST25VF_H
#define	SST25VF_H

#include <stdint.h>
#include <stdbool.h>
#include "SST25xx_Cfg.h"

typedef enum SST_PROTECT_RANGE
{
    FLASH_PROTECT_NONE = 0,
    FLASH_PROTECT_64KB = 1,
    FLASH_PROTECT_128KB = 2,
    FLASH_PROTECT_256KB = 3,
    FLASH_PRTOECT_512KB = 4,
    FLASH_PROTECT_1MB = 5,
    FLASH_PROTECT_2MB = 6,
    FLASH_PROTECT_ALL = 7
} sst_protect_range_t;

/*************************************************************PUBLIC FUNCTIONS*/
// address= 0x00000 to MAX_MEM_ADDR
// range= see in FLASH_PROTECT_RANGE
// len=1 to 256, if over 256 bytes, this function will not execute

void SST_Chip_Erase(void); // Erase full-memory
void SST_Sector_Erase(uint32_t BAddr); // Erase a sector
void SST_Read_nByte(uint32_t BAddr, uint32_t len, uint8_t *buffer); // Read n byte(s)
uint8_t SST_Read_Byte(uint32_t BAddr); // Read 1 byte
void SST_Write_nByte(uint32_t BAddr, uint16_t len, uint8_t *data); // Write n byte(s)
void SST_Write_Byte(uint32_t Addr, uint8_t data); // Write 1 byte
void SST_Protect(sst_protect_range_t range); // Protect memory zone
uint32_t SST_Get_JDECID(void); // Get JEDECID
bool SST_Init(void); // Call this function first (SPI must be initialized before)

#endif