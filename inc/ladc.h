#include "cl_common.h"

typedef enum
{
    AdcChan_Current,
    AdcChan_ExtVol,
    AdcChan_Battery0,
    AdcChan_Battery1,
} AdcChannel_t;

void Adc_Init(void);
int16_t GetAdcResult(AdcChannel_t chan);
