#ifndef SST25XX_H
#define SST25XX_H

/* *********************************************************** Chip selection */
//#define SST25VF020B                                                             // 2Mbit Flash  <-------------------- User can redefine this line
#define SST25PF040B                                                             // 4Mbit Flash  <-------------------- User can redefine this line
//#define SST25VF016B                                                             // 4Mbit Flash  <-------------------- User can redefine this line

/* ********************************************************************** SPI */
#include "spi1_driver.h"
#include "pin_manager.h"

#define FLASH_CS_N_SetLow() CS_SetLow() 
#define FLASH_CS_N_SetHigh() CS_SetHigh()
#define SPI_Exchange8bit(data)  spi1_exchangeByte(data)

static inline void SPI_Init(void) 
{
    FLASH_CS_N_SetLow();
    spi1_close();
    spi1_open(MASTER0_CONFIG);
    FLASH_CS_N_SetHigh();
}

#endif
