#pragma once
#include "stdbool.h"

void BreathRgb_Init(void);
void BreathRgb_Update(void);
void BreathRgb_Enable(bool en);

typedef enum
{
    BLF_Freq0 = 0,
    BLF_Freq1,
    BLF_Freq2,
    BLF_Freq3,
    BLF_Freq4,
    BLF_FreqMax,
} BreathLedFreq_t;
void BreatRgb_SetFreq(BreathLedFreq_t rFreq, BreathLedFreq_t gFreq, BreathLedFreq_t bFreq);
