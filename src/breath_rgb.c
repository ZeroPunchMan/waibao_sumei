#include "breath_rgb.h"
#include "systime.h"
#include "string.h"
#include "nrf_gpio.h"

#define RGB_MAX_LEVEL (40)

static volatile uint8_t rgbLevel[3] = {0, 0, 0};
static volatile uint8_t rgbDir[3] = {1, 1, 1};
static volatile bool initialized = false;
static volatile uint8_t rgbEnable = false;

void BreathRgb_Init(void)
{
    initialized = true;
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(0, 25)); // r
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(0, 26)); // g
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(0, 27)); // b

    BreathRgb_Enable(true);
}

void BreathRgb_Enable(bool en)
{
    if (en)
    {
        rgbEnable = true;
    }
    else
    {
        rgbEnable = false;
        nrf_gpio_pin_set(NRF_GPIO_PIN_MAP(0, 25));
        nrf_gpio_pin_set(NRF_GPIO_PIN_MAP(0, 26));
        nrf_gpio_pin_set(NRF_GPIO_PIN_MAP(0, 27));

        for (int i = 0; i < 3; i++)
        {
            rgbLevel[i] = 0;
        }
    }
}

void BreathRgb_Update(void)
{
    if (!initialized || !rgbEnable)
        return;

    // 亮度变化,每100ms变化一次
    static uint32_t levelTime = 0;
    if (SysTimeSpan(levelTime) >= 30) // todo
    {
        levelTime = GetSysTime();

        for (int i = 0; i < 3; i++)
        {
            if (rgbLevel[i] >= RGB_MAX_LEVEL)
                rgbDir[i] = 0;
            else if (rgbLevel[i] == 0)
                rgbDir[i] = 1;

            if (rgbDir[i])
                rgbLevel[i]++;
            else
                rgbLevel[i]--;
        }
    }

    // 模拟PWM,每次都更新
    static uint8_t rgbCount[3] = {0, 0, 0};
    for (int i = 0; i < 3; i++)
    {
        rgbCount[i]++;
        if (rgbCount[i] >= RGB_MAX_LEVEL)
            rgbCount[i] = 0;
    }

    // R
    if (rgbCount[0] < rgbLevel[0])
        nrf_gpio_pin_clear(NRF_GPIO_PIN_MAP(0, 25));
    else
        nrf_gpio_pin_set(NRF_GPIO_PIN_MAP(0, 25));
        
    // G
    if (rgbCount[1] < rgbLevel[1])
        nrf_gpio_pin_clear(NRF_GPIO_PIN_MAP(0, 26));
    else
        nrf_gpio_pin_set(NRF_GPIO_PIN_MAP(0, 26));

    // B
    if (rgbCount[2] < rgbLevel[2])
        nrf_gpio_pin_clear(NRF_GPIO_PIN_MAP(0, 27));
    else
        nrf_gpio_pin_set(NRF_GPIO_PIN_MAP(0, 27));
}
