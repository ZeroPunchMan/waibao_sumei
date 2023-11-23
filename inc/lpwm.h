#pragma once

#include "cl_common.h"

typedef enum
{
    PwmChan_Mos0 = 0, 
    PwmChan_Mos1,
    PwmChan_Mos2,
    PwmChan_Mos3,
} PwmChannel_t;

void Pwm_Init(void);
void Pwm_SetOutput(PwmChannel_t chan, uint8_t duty);
