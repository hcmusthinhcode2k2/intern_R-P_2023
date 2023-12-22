#ifndef     LED7SEG_CFG_H
#define     LED7SEG_CFG_H

#include "mcc_generated_files/pin_manager.h"

#define   NUM_OF_7SEG_DIGIT           4
#define   LED_7SEG_ACT_TIME           2
#define   LED_7SEG_TOGGLE_COUNT       30

void Led7Seg_Digit_SetState(uint8_t x);
void Led7Seg_Digit_Disable(void);
void Led7Seg_Segment_SetState(uint8_t x);
void Led7Seg_Segment_Disable(void);

#endif