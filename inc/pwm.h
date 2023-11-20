#pragma once

#include "cl_common.h"

typedef enum
{
    PwmChan_0,
    PwmChan_1,
    PwmChan_2,
    PwmChan_3,
} PwmChannel_t;

void Pwm_Init(void);
void Pwm_SetOutput(uint8_t duty);
