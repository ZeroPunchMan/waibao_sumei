#include "sys_output.h"
#include "systime.h"
#include "lpwm.h"
#include "nrf_gpio.h"
#include "cl_log.h"
#include "bat_monitor.h"
#include "led.h"

#define OUTPUT_EN_PIN (NRF_GPIO_PIN_MAP(0, 2))

static bool runPause = false;
static uint8_t power[4] = {0};
void SysOutput_Init(void)
{
    nrf_gpio_cfg_output(OUTPUT_EN_PIN);
    nrf_gpio_pin_clear(OUTPUT_EN_PIN);
}

void SysOutput_Process(void)
{
    if (!runPause)
    {
        SetBreathLed(BreathLed_VerySlow);
    }
    else
    {
        uint32_t sumOfPwr = 0;
        for (int i = 0; i < CL_ARRAY_LENGTH(power); i++)
            sumOfPwr += power[i];

        sumOfPwr /= 80;
        switch (sumOfPwr)
        {
        case 0:
            SetBreathLed(BreathLed_VerySlow);
            break;
        case 1:
            SetBreathLed(BreathLed_Slow);
            break;
        case 2:
            SetBreathLed(BreathLed_Normal);
            break;
        case 3:
            SetBreathLed(BreathLed_Fast);
            break;
        default:
            SetBreathLed(BreathLed_VeryFast);
            break;
        }
    }
}

void SysOutput_Stop()
{
    SysOutput_RunPause(false);
    nrf_gpio_pin_clear(OUTPUT_EN_PIN);
    CL_LOG("stop output");
}

void SysOutput_RunPause(bool run)
{
    if (!run)
    {
        MosRunPause(false);
        runPause = run;
    }
    else
    {
        if (GetBatStatus() != BatSta_Ok)
            return;

        nrf_gpio_pin_set(OUTPUT_EN_PIN); 
        MosRunPause(true);
        runPause = run;
    }
}

void SysOutput_SetChannel(uint8_t chan, uint8_t p, uint8_t f)
{
    p = CL_CLAMP(p, 0, 100);
    f = CL_CLAMP(f, 6, 10);

    power[chan] = p;
    MosChanSet(chan, p, f);
    CL_LOG("chann%d: %d--%d", chan, p, f);
}

uint32_t GetSysOutput(void)
{
    return runPause;
}
