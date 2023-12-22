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
#include "mcc_generated_files/adc1.h"
#include "mcc_generated_files/uart1.h"
#ifndef FCY
#define FCY (_XTAL_FREQ / 2)
#endif
#include <libpic30.h>
#include <inttypes.h>
/*
                         Main application
 */
#include "mcc_generated_files/mcc.h"
#include <inttypes.h>
#include "pcm.h"

//declare variables
static volatile uint32_t jiffies=0;
static volatile uint16_t counter=0;
static const ADC1_CHANNEL channel=channel_AN23;
// ADC is 12-bit
//static const uint16_t MAX_ADC = 0xFFF;

static uint16_t max_duty=0;
//set timer function

static void TMR1_CallBack(void)
{
    //    static bool phase=0;
    //    static int32_t duty=0;
    //    LED1_Toggle();
    //    LED2_Toggle();
    //    //    counter++;
    //    //    jiffies++;
    //
    //    if(phase==0)
    //    {
    //        duty+=128;
    //
    //        if(duty>PG1PER)
    //        {
    //            duty-=128;
    //            phase=1;
    //        }
    //    }
    //    else
    //    {
    //        duty-=128;
    //
    //        if(duty<0)
    //        {
    //            duty+=128;
    //            phase=0;
    //        }
    //    }

    static uint32_t i=0;

    if(i<NUM_ELEMENTS)
        PG1DC=(uint16_t) data[i];
    else
        PG1DC=0;

    i++;
    PG1STATbits.UPDREQ=1; // Update request set
    while(!PG1STATbits.UPDATE);
}
//read ADC function

uint16_t readADC(void)
{
    uint16_t conversion;
    int i=0;
    ADC1_SoftwareTriggerEnable();
    for(i=0; i<1000; i++)
    {
        //delay
    }
    ADC1_SoftwareTriggerDisable();
    while(!ADC1_IsConversionComplete(channel));
    conversion=ADC1_ConversionResultGet(channel);
    return conversion;
}

int main(void)
{
    uint16_t pwm_duty_old=0;
    //    uint32_t tmp = 0;
    SYSTEM_Initialize();
    //    printf("1.Init\r\n");
    //    printf("Version: %d.%02d\r\n",APP_VERSION/100, APP_VERSION%100);
    //master period
    max_duty=MPER; // 16 dec -> 2Hz 
    printf("max_duty=%" PRIu16 "\r\n", max_duty);
    LED1_SetHigh();
    LED2_SetLow();
    ADC1_Enable();
    ADC1_ChannelSelect(channel);
    pwm_duty_old=PG1DC;
    PWM_GeneratorEnable(PWM_GENERATOR_1);
    TMR1_SetInterruptHandler(&TMR1_CallBack);
    //    printf("1.Main-Loop\r\n");
    while(1)
    {
        //        //        uint16_t pwm_duty = 0;
        //        uint16_t conversion=readADC();
        //        uint16_t pwm_duty=16*conversion;
        //        //        tmp = conversion & MAX_ADC;
        //        //        pwm_duty = conversion * max_duty / MAX_ADC;
        //        //        tmp = tmp * max_duty / MAX_ADC;
        //        //        pwm_duty = (uint16_t)tmp;
        //        if(pwm_duty!=pwm_duty_old)
        //        {
        //            PG1DC=pwm_duty;
        //            PG1STATbits.UPDREQ=1; // Update request set
        //            while(!PG1STATbits.UPDATE);
        //            pwm_duty_old=pwm_duty;
        //        }
        //        // gia tri in va khong lam tran man hinh teminal
        //        if(counter>=4)
        //        {
        //            printf("Uptime ticks: %" PRIu32 "\r\n", jiffies);
        //            printf("  ADC=%" PRIu16 " (0x%" PRIx16 ")\r\n", conversion, conversion);
        //            printf("  PWM_Duty=%" PRIu16 " (0x%" PRIx16 ")\r\n", pwm_duty, pwm_duty);
        //            counter=0;
        //        }

        int i;


    }

    return 1;
}



//static volatile uint16_t dutyCycle=0;
//
//void PWM_duty_update(void)
//{
//    dutyCycle+=1024;
//    PWM_Disable();
//    PWM_DutyCycleSet(PWM_GENERATOR_1, dutyCycle);
//    PWM_Enable();
//    LED2_Toggle();
//}
//
///*
//                         Main application
// */
//int main(void)
//{
//    // initialize the device
//    SYSTEM_Initialize();
//    dutyCycle=0;
//    TMR1_SetInterruptHandler(PWM_duty_update);
//
//    while(1)
//    {
//        // Add your application code
//    }
//    return 1;
//}