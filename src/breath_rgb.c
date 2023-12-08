#include "breath_rgb.h"
#include "systime.h"
#include "string.h"
#include "nrf_gpio.h"

#define RGB_MAX_LEVEL (40)

#define RED_PIN (NRF_GPIO_PIN_MAP(0, 26))
#define GREEN_PIN (NRF_GPIO_PIN_MAP(0, 27))
#define BLUE_PIN (NRF_GPIO_PIN_MAP(0, 20))

static volatile uint8_t rgbLevel[3] = {0, 0, 0};
static volatile uint8_t rgbDir[3] = {1, 1, 1};
static volatile bool initialized = false;
static volatile uint8_t rgbEnable = false;
static volatile uint8_t rgbBreathIntvl[3] = {30, 30, 30};

void BreathRgb_Init(void)
{
    initialized = true;
    nrf_gpio_cfg_output(RED_PIN); // r
    nrf_gpio_cfg_output(GREEN_PIN); // g
    nrf_gpio_cfg_output(BLUE_PIN); // b

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
        nrf_gpio_pin_set(RED_PIN);
        nrf_gpio_pin_set(GREEN_PIN);
        nrf_gpio_pin_set(BLUE_PIN);

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

    // 亮度变化
    static uint32_t levelTime = 0;
    if (SysTimeSpan(levelTime) >= rgbBreathIntvl[0]) 
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
        nrf_gpio_pin_clear(RED_PIN);
    else
        nrf_gpio_pin_set(RED_PIN);

    // G
    if (rgbCount[1] < rgbLevel[1])
        nrf_gpio_pin_clear(GREEN_PIN);
    else
        nrf_gpio_pin_set(GREEN_PIN);

    // B
    if (rgbCount[2] < rgbLevel[2])
        nrf_gpio_pin_clear(BLUE_PIN);
    else
        nrf_gpio_pin_set(BLUE_PIN);
}

static const uint8_t freqIntvlTable[BLF_FreqMax] = {50, 45, 40, 35, 30};
void BreatRgb_SetFreq(BreathLedFreq_t rFreq, BreathLedFreq_t gFreq, BreathLedFreq_t bFreq)
{
    rgbBreathIntvl[0] = freqIntvlTable[rFreq];
    rgbBreathIntvl[1] = freqIntvlTable[gFreq];
    rgbBreathIntvl[2] = freqIntvlTable[bFreq];
}
