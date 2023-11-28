#include "sys_output.h"
#include "systime.h"
#include "lpwm.h"
#include "nrf_gpio.h"
#include "cl_log.h"
#include "bat_monitor.h"
#include "breath_rgb.h"


void SysOutput_Init(void)
{
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(0, 11));
    nrf_gpio_pin_clear(NRF_GPIO_PIN_MAP(0, 11));
}

void SysOutput_Process(void)
{

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
        MosRunPause(false);

    }
    else
    {
        if (GetBatStatus() != BatSta_Ok)
            return;

        nrf_gpio_pin_set(NRF_GPIO_PIN_MAP(0, 11));
        MosRunPause(true);
    }
}

void SysOutput_SetChannel(uint8_t chan, uint8_t power, uint8_t freq)
{
    power = CL_CLAMP(power, 0, 100);
    freq = CL_CLAMP(freq, 6, 10);

    MosChanSet(chan, power, freq);
    CL_LOG("chann%d: %d--%d", chan, power, freq);
}
