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
#include "mcc_generated_files/adc1.h"
#include <inttypes.h>
#ifndef FCY
#define FCY (_XTAL_FREQ/2)
#endif
#include <libpic30.h>
/*
                         Main application
 */
static const ADC1_CHANNEL channel=channel_AN23;
static volatile uint16_t counter = 1000;
static void TMR1_CallBack(void)
{
    LED1_Toggle();
    LED2_Toggle();
}

uint16_t readADC()
{
    uint16_t conversion;
    ADC1_SoftwareTriggerEnable();
    for(int i=0;i<1000;i++)
    {
           //delay;
    }
    ADC1_SoftwareTriggerDisable();
    while(!ADC1_IsConversionComplete(channel));
    conversion=ADC1_ConversionResultGet(channel);
    return conversion;
    
}
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    LED1_SetHigh();
    LED2_SetLow();
    ADC1_Enable();
    LED1_SetHigh();
    LED2_SetLow();
    uint16_t coversionOld=0;
    ADC1_ChannelSelect(channel);
    TMR1_SetInterruptHandler(&TMR1_CallBack);
    while (1)
    {
        // Add your application code
        uint16_t conversionNew=readADC();
        if (coversionOld<conversionNew)
        {
                 counter=counter+100;
        }
        coversionOld=conversionNew;
        LED_ADC_SetHigh();
        __delay_ms(counter);
        LED_ADC_SetLow();
          printf("  ADC=%" PRIu16 " (0x%" PRIx16 ")\r\n", conversionNew, conversionNew);
          __delay_ms(1000);
          printf("  Counter=%" PRIu16 " (0x%" PRIx16 ")\r\n", counter, counter);
          __delay_ms(1000);
    }
    return 1; 
}
/**
 End of File
*/

