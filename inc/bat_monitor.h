#pragma once

#include "cl_common.h"

#define VOLTAGE_TO_ADC(vol) ((vol)*1000 / 3.6f)

typedef enum
{
    BatSta_Ok = 1,
    BatSta_Low = 2,
    BatSta_Charge = 3,
    BatSta_ChargeFull = 4,
} BatStatus_t;

void BatMonitor_Init(void);
void BatMonitor_Process(void);

BatStatus_t GetBatStatus(void);
uint8_t GetBatPercent(void);
