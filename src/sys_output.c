#include "sys_output.h"
#include "systime.h"
#include "lpwm.h"
#include "nrf_gpio.h"
#include "cl_log.h"
#include "bat_monitor.h"
#include "breath_rgb.h"

typedef struct
{
    uint8_t power; // 0~100
    uint8_t freq;  // 6~10
    bool work;
    uint32_t lastStepTime;
} SysChanContext_t;

SysChanContext_t chanCtxs[4];
static bool sysRunPause = false;

static const uint32_t freqTable[5] = {83, 71, 63, 56, 50};

void SysOutput_Init(void)
{
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(0, 11));
    for (int i = 0; i < CL_ARRAY_LENGTH(chanCtxs); i++)
    {
        chanCtxs[i].power = 0;
        chanCtxs[i].freq = 0;
        chanCtxs[i].work = false;
        chanCtxs[i].lastStepTime = 0;
    }
}

void SysOutput_Process(void)
{
    if (sysRunPause)
    {
        uint16_t sumOfPwr = 0;
        for (int i = 0; i < CL_ARRAY_LENGTH(chanCtxs); i++)
        {
            sumOfPwr += chanCtxs[i].power;
            uint32_t interval = freqTable[chanCtxs[i].freq - 6];
            if (SysTimeSpan(chanCtxs[i].lastStepTime) >= interval)
            {
                chanCtxs[i].lastStepTime = GetSysTime();

                chanCtxs[i].work = !chanCtxs[i].work;
                if (chanCtxs[i].work)
                    Pwm_SetOutput((PwmChannel_t)(i + PwmChan_Mos0), chanCtxs[i].power); //todo 用100占空比验证频率
                else
                    Pwm_SetOutput((PwmChannel_t)(i + PwmChan_Mos0), 0);
            }
        }

        sumOfPwr /= 80;
        sumOfPwr = CL_MIN(sumOfPwr, BLF_Freq4);
        BreatRgb_SetFreq(sumOfPwr, sumOfPwr, sumOfPwr);
        // todo 呼吸灯
    }
}

void SysOutput_Stop()
{
    SysOutput_RunPause(false);
    nrf_gpio_pin_clear(NRF_GPIO_PIN_MAP(0, 11));
}

void SysOutput_RunPause(bool run)
{
    if (!run)
    {
        for (int i = 0; i < CL_ARRAY_LENGTH(chanCtxs); i++)
            Pwm_SetOutput((PwmChannel_t)(i + PwmChan_Mos0), 0);

        BreatRgb_SetFreq(BLF_Freq0, BLF_Freq0, BLF_Freq0);
        sysRunPause = false;
    }
    else
    {
        if (GetBatStatus() != BatSta_Ok)
            return;

        nrf_gpio_pin_set(NRF_GPIO_PIN_MAP(0, 11));
        sysRunPause = true;
    }
}

void SysOutput_SetChannel(uint8_t chan, uint8_t power, uint8_t freq)
{
    power = CL_CLAMP(power, 0, 100);
    freq = CL_CLAMP(freq, 6, 10);
    chanCtxs[chan].power = power;
    chanCtxs[chan].freq = freq;

    CL_LOG("chann%d: %d--%d", chan, power, freq);
}
