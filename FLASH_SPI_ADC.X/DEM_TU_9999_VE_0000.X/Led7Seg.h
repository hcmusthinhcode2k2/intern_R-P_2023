
#ifndef   LED7SEG_H
#define   LED7SEG_H
#include <stdint.h>
#include <stdbool.h>
#include "Led7Seg_Cfg.h"

extern const uint8_t Led7SegCode[];

void Led7Seg_Init(void);
void Led7Seg_Tasks(void);
// void Led7Seg_SetDigitValue(uint8_t DgIdx,uint8_t Value);
void Led7Seg_DisplayInteger(uint32_t Value);

#endif