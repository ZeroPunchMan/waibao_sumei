#include "sys_output.h"
#include "systime.h"
#include "lpwm.h"
#include "nrf_gpio.h"
#include "cl_log.h"
#include "bat_monitor.h"

#define OUTPUT_EN_PIN (NRF_GPIO_PIN_MAP(0, 2))

static bool runPause = false;
void SysOutput_Init(void)
{
    nrf_gpio_cfg_output(OUTPUT_EN_PIN);
    nrf_gpio_pin_clear(OUTPUT_EN_PIN);
}

void SysOutput_Process(void)
{
}

void SysOutput_Stop()
{
    SysOutput_RunPause(false);
    nrf_gpio_pin_clear(OUTPUT_EN_PIN);
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

void SysOutput_SetChannel(uint8_t chan, uint8_t power, uint8_t freq)
{
    power = CL_CLAMP(power, 0, 100);
    freq = CL_CLAMP(freq, 6, 10);

    MosChanSet(chan, power, freq);
    CL_LOG("chann%d: %d--%d", chan, power, freq);
}

uint32_t GetSysOutput(void)
{
    return runPause;
}
