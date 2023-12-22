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




// Kh?i t?o MAX6675
void Max6675_Init(void) {
    CS_SetDigitalOutput();
    CS_SetHigh();

    spi1_open(1);
}

// ??c nhi?t ?? t? MAX6675
uint16_t Max6675_Read_Temp(void) {
    uint16_t tempData = 0;
    uint8_t highByte, lowByte;

    CS_SetLow();
    __delay_ms(1);

    highByte = spi1_exchangeByte(0xFF); 
    lowByte = spi1_exchangeByte(0xFF); 

    CS_SetHigh();

    tempData = (highByte << 8) | lowByte;
    if (tempData & 0x4) {
        return 0xFFFF;
    }
    tempData >>= 3; 

    return tempData * 0.25;
}

int main(void) {

    SYSTEM_Initialize();
    Max6675_Init();

    while (1) {
        uint16_t temperature = Max6675_Read_Temp();
        if (temperature != 0xFFFF) {
            printf("Temperature: %u C\n", temperature);
        } else {
            printf("Thermocouple connection error\n");
        }
        __delay_ms(1000);
    }
    return 1;
}
