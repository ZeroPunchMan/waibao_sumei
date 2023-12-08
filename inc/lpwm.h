#pragma once

#include "cl_common.h"

typedef enum
{
    PwmChan_Led0 = 0, 
    PwmChan_Led1,
} PwmChannel_t;

void Pwm_Init(void);

void MosRunPause(bool run);
void MosChanSet(uint8_t chan, uint8_t power, uint8_t freq);

void SetPwmChan(PwmChannel_t chan, uint8_t duty);

