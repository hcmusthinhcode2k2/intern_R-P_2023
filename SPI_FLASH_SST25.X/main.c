/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.4
        Device            :  dsPIC33CK256MP508
    The generated drivers are tested against the following:
        Compiler          :  XC16 v2.10
        MPLAB 	          :  MPLAB X v6.05
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/
#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/uart1.h"
#include "SST25xx.h"
#include "SPI_MAX6675.h"
#ifndef FCY
#define FCY (_XTAL_FREQ / 2)
#endif
#include <libpic30.h>
#include <string.h>

#define AM2320_ADDRESS  0xB8>>1
//uint16_t readAddress(void)
//{
//    uint16_t readSensorData(uint8_t regAddr) 
//    uint8_t reg = regAddr;
//    uint8_t data[2] = {0};
//    i2c_writeNBytes(AM2320_ADDRESS, &reg, 1);
//    i2c_readNBytes(AM2320_ADDRESS, data, 2);
//    return (data[1] << 8) | data[0];
//}
unsigned int CRC16(uint8_t *ptr, uint8_t length) {
    unsigned int crc = 0xFFFF;
    uint8_t s = 0x00;
    while (length--) {
        crc ^= *ptr++;
        for (s = 0; s < 8; s++) {
            if ((crc & 0x01) != 0) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}
void initSensor(void) 
{
    uint8_t wake[1] = {0x00};
    uint8_t cmd[3] = {0x03, 0x00, 0x04};
    i2c_writeNBytes(AM2320_ADDRESS, wake, 1);
    i2c_writeNBytes(AM2320_ADDRESS, cmd, sizeof(cmd));

}
uint8_t readSensorData(uint8_t *data) {
    uint8_t cmd[3] = {0x03, 0x00, 0x04};
    i2c_writeNBytes(AM2320_ADDRESS, cmd, sizeof(cmd));
    i2c_readNBytes(AM2320_ADDRESS, data, 8);
    if (data[0] != 0x03 || data[1] != 0x04) {
        return 1; 
    }
    unsigned int receivedCrc = (data[7] << 8) | data[6];
    if (CRC16(data, 6) != receivedCrc)
    {
        return 2; 
    }
    return 0; 
}
void getTemperatureAndHumidity(uint16_t *temperature, uint16_t *humidity) {
    uint8_t data[8];
    if (readSensorData(data) == 0) {
        humidity = ((uint16_t)data[2] << 8) | data[3];
        uint16_t rawTemperature = ((uint16_t)data[4] << 8) | data[5];
        if (rawTemperature & 0x8000) { 
            *temperature = -((int16_t)(rawTemperature & 0x7FFF));
        } else {
            *temperature = rawTemperature;
        }
    } else {
        *temperature = 0;
        *humidity = 0;
    }
}

int main(void) {
    SYSTEM_Initialize();
    SST_Init();
    initSensor();
    while (1) 
    {
        //        SST_Sector_Erase(0x100);
//        const char dataWrite1[]= "check";
//        uint8_t readBufer[10];
//        SST_Sector_Erase(0x100);
//        SST_Write_nByte(0x100,strlen(dataWrite1)+1,dataWrite1);
//        SST_Read_nByte(0x100,strlen(dataWrite1)+1,readBufer);
//        printf("Read Data %s",readBufer);
//        printf("\n");
//        __delay_ms(1000);
        uint8_t humidity, temperature;
        if (getTemperatureAndHumidity(&humidity, &temperature)) 
        {
            printf("Humidity: %.1u%%, Temperature: %.1u°C\r\n", humidity, temperature);
            __delay_ms(1000);
        } else {
            printf("loi du lieu\r\n");
            __delay_ms(1000);
        }
        
//        printf("check\n");
//        __delay_ms(1000);
    }

    return 1;
}

