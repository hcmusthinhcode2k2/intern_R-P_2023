#ifndef SPI_MAX6675_H
#define SPI_MAX6675_H


#include "../mcc_generated_files/mcc.h"
#ifndef FCY
#define FCY (_XTAL_FREQ / 2)
#endif
#include <libpic30.h>


void Max6675_Init(void) ;
uint16_t Max6675_Read_TempC(void);
uint16_t  Max6675_Read_TempF(void);
#endif