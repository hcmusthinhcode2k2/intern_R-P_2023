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
#ifndef FCY
#define FCY (_XTAL_FREQ / 2)
#endif
#include <libpic30.h>
#include <stdlib.h>

#define    RPR0521RS_ADDR                             (0x38)
#define    RPR0521RS_PART_ID_VAL                      (0x0A)
#define    RPR0521RS_MANUFACT_ID_VAL                  (0xE0)

#define    RPR0521RS_SYSTEM_CONTROL                   (0x40)
#define    RPR0521RS_MODE_CONTROL                     (0x41)
#define    RPR0521RS_ALS_PS_CONTROL                   (0x42)
#define    RPR0521RS_PS_CONTROL                       (0x43)
#define    RPR0521RS_PS_DATA_LSB                      (0x44)
#define    RPR0521RS_ALS_DATA0_LSB                    (0x46)
#define    RPR0521RS_MANUFACT_ID                      (0x92)
/*
                         Main application
 */
void initSensor(void) 
{
    uint8_t modeControlVal = RPR0521RS_MODE_CONTROL ;
    uint8_t psControlVal   = RPR0521RS_PS_CONTROL  ;
    i2c_writeNBytes(RPR0521RS_ADDR,modeControlVal,1);
    i2c_writeNBytes(RPR0521RS_ADDR,psControlVal,1);
}
uint16_t readSensorData(uint8_t regAddr) 
{
    uint8_t reg = regAddr;
    uint8_t data[2] = {0};
    i2c_writeNBytes(RPR0521RS_ADDR, &reg, 1);
    i2c_readNBytes(RPR0521RS_ADDR, data, 2);
    return (data[1] << 8) | data[0];
}
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    initSensor();
    
    while (1)
    {
        // Add your application cod
        uint16_t value=readSensorData(RPR0521RS_PS_DATA_LSB);
        printf("%d\n",value);
//        printf("test\n");
        __delay_ms(100);
                
    }
    return 1; 
}
/**
 End of File
*/

