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
#ifndef FCY
#define FCY (_XTAL_FREQ/2)
#endif
#include <libpic30.h>
#include "mcc_generated_files/adc1.h"
#include "mcc_generated_files/uart1.h"
#include "stick/SystemTick.h"
#include "oled/OLED.h"
#include "SST25/SST25xx.h"
#include "string.h"
#include <stdio.h>
static const ADC1_CHANNEL channel=channel_AN23;
#define ADC_START_ADDRESS 0x10000 
#define ADC_MAX_ADDRESS   0x20000 // Gi?i h?n trên cho ??a ch?
#define MAX_SAMPLES 1000
uint32_t address = ADC_START_ADDRESS; // Bi?n toàn c?c ?? l?u tr? ??a ch?
uint16_t readADC(void)
{
    uint16_t conversion;
    int i=0;
    ADC1_SoftwareTriggerEnable();
    for (i=0;i<1000;i++)
    {
        //delay
    }
    ADC1_SoftwareTriggerDisable();
    while(!ADC1_IsConversionComplete(channel));
    conversion=ADC1_ConversionResultGet(channel);
    return conversion;
}

/*
                         Main application
 */

int main(void) {
    SYSTEM_Initialize();
    SystemTick_Init();
    ADC1_Enable();
    ADC1_ChannelSelect(channel);
    SST_Init();
    uint16_t sampleCount = 0; 
    while (1) 
    {
       
//        const char dataWrite1[]= "check";
//        uint8_t readBufer[10];
//        SST_Sector_Erase(0x100);
//        SST_Write_nByte(0x100,strlen(dataWrite1)+1,dataWrite1);
//        SST_Read_nByte(0x100,strlen(dataWrite1)+1,readBufer);
//        printf("Read Data %s",readBufer);
//        printf("\n");
//        __delay_ms(1000);
         uint16_t adcValue = readADC();
//         SST_Sector_Erase(0x100);
//         printf("check: %d\n",adcValue);
//         __delay_ms(100);
         //mode 1       
         if (S1_GetValue() == 0)
         {
//               ADC_START_ADDRESS++;
                SST_Sector_Erase(address);
                uint8_t adc1=(adcValue >> 0) & 0xFF;  
                uint8_t adc2=(adcValue >> 8) & 0xFF;
                SST_Write_Byte(address,adc1); 
                SST_Write_Byte(address + 1, adc2);  
                SST_Read_Byte(address);
                SST_Read_Byte(address+1);
                address += 2; 
                UART1_Write(0x03);
                UART1_Write(adc1);
                UART1_Write(adc2);
                UART1_Write(0xFC);
                __delay_ms(100);
         }
         //mode 2
         if (S2_GetValue() == 0)
         { 
             if (sampleCount < MAX_SAMPLES) 
             {
                uint32_t address = ADC_START_ADDRESS;
                SST_Sector_Erase(address);
                SST_Write_Byte(address, adcValue); 
                SST_Read_Byte(address);
                printf("MODE 2 Save ADC value: %u address: 0x%X\n", adcValue, address); 
                sampleCount++;
             } else 
             {
                printf("too much.\n");
             }
             while(S2_GetValue() == 0)
             { 
                __delay_ms(10);
             }
            sampleCount = 0;
        }
        __delay_ms(100)   
        if (S3_GetValue()==0)
        {
            SST_Chip_Erase();
            __delay_ms(100);
        }
    }
    return 1;
}

/**
 End of File
*/

