#pragma once

#include "cl_common.h"

typedef enum
{
    LedIdx_Ble,
    LedIdx_Charge,
    LedIdx_Breath,
} LedIndex_t;

void Led_Init(void);
void Led_Process(void);


typedef enum
{
    BleLedStyle_Blink,
    BleLedStyle_On,
} BleLedStyle_t;
void SetBleLed(BleLedStyle_t);


typedef enum
{
    ChgLedStyle_None,
    ChgLedStyle_Charging,
    ChgLedStyle_ChargeDone,
    ChgLedStyle_Work,
} ChargeLedStyle_t;

void SetChargeLed(ChargeLedStyle_t);

typedef enum
{
    BreathLed_Slow,
    BreathLed_Normal,
    BreathLed_Fast,
} BreathLedStyle_t;
void SetBreathLed(BreathLedStyle_t);
