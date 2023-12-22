#include "SPI_MAX6675.h"


void Max6675_Init(void) 
{
    CS1_SetDigitalOutput();
    CS1_SetHigh();
}
uint16_t Max6675_Read_TempC(void)
{
    uint16_t tempData = 0;
    uint8_t highByte, lowByte;
    CS1_SetLow();
    __delay_us(200);
    highByte = spi1_exchangeByte(0xFF);
    lowByte = spi1_exchangeByte(0xFF);
    CS1_SetHigh();
    tempData = (highByte << 8) | lowByte;
    if (tempData & 0x4) 
    {
        return 0xFFFF;
    }
    tempData >>= 3;
    return tempData * 0.22;
}
uint16_t  Max6675_Read_TempF(void) 
{ 
    return  Max6675_Read_TempC() * 9.0 / 5.0 + 32;
}