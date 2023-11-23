#include "bat_monitor.h"
#include "ladc.h"
#include "nrf_gpio.h"
#include "cl_log.h"
#include "systime.h"
#include "sys_output.h"

#define EXT_VOL_THRESH (VOLTAGE_TO_ADC(0.8f))

static BatStatus_t batStatus = BatSta_Ok;
static uint8_t batPercent = 100;
static int16_t extVolAdc = 0;

void BatMonitor_Init(void)
{
    nrf_gpio_cfg_input(NRF_GPIO_PIN_MAP(0, 14), NRF_GPIO_PIN_PULLUP); // 充电检测
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(0, 9));                      // 充电指示灯1
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(0, 10));                     // 充电指示灯2
}

static inline bool IsChargeFull(void)
{
    return nrf_gpio_pin_read(NRF_GPIO_PIN_MAP(0, 14)) == 0;
}

typedef enum
{
    ChgSta_None,
    ChgSta_NotFull,
    ChgSta_Full,
} ChargeSta_t;
static inline void SetChargeIndLed(ChargeSta_t sta)
{
    switch (sta)
    {
    case ChgSta_None:
        nrf_gpio_pin_clear(NRF_GPIO_PIN_MAP(0, 9));
        nrf_gpio_pin_clear(NRF_GPIO_PIN_MAP(0, 10));
        break;
    case ChgSta_NotFull:
        nrf_gpio_pin_set(NRF_GPIO_PIN_MAP(0, 9));
        nrf_gpio_pin_clear(NRF_GPIO_PIN_MAP(0, 10));
        break;
    case ChgSta_Full:
        nrf_gpio_pin_clear(NRF_GPIO_PIN_MAP(0, 9));
        nrf_gpio_pin_set(NRF_GPIO_PIN_MAP(0, 10));
        break;
    }
}

static void ToLow(void);
static void ToCharge(void);
static void ToOk(void)
{
    batStatus = BatSta_Ok;
    SetChargeIndLed(ChgSta_None);
    CL_LOG("bat ok");
}

static void OkProc(void)
{
    if (extVolAdc > EXT_VOL_THRESH)
    {
        ToCharge();
    }
    else if (batPercent <= 30)
    {
        ToLow();
    }
}

static uint32_t lowStartTime = 0;
static bool stopOutput = false;
static void ToLow(void)
{
    batStatus = BatSta_Low;
    stopOutput = false;
    lowStartTime = GetSysTime();
    SetChargeIndLed(ChgSta_None);
    CL_LOG("bat low");
}

static void LowProc(void)
{
    if (extVolAdc > EXT_VOL_THRESH)
    {
        ToCharge();
    }
    else
    {
        if (!stopOutput)
        {
            if (SysTimeSpan(lowStartTime) >= SYSTIME_SECOND(20)) // todo 修改时间
            {
                stopOutput = true;
                SysOutput_Stop();
                CL_LOG("low bat, stop output");
            }
        }
        if(batPercent >= 50)
        {
            ToOk();
        }
    }
}

static void ToCharge(void)
{
    batStatus = BatSta_Charge;
    CL_LOG("bat charge");
}

static void ChargeProc(void)
{
    if (extVolAdc > EXT_VOL_THRESH)
    {
        if (IsChargeFull())
            SetChargeIndLed(ChgSta_Full);
        else
            SetChargeIndLed(ChgSta_NotFull);
    }
    else
    {
        ToOk();
    }
}

static const int16_t batAdcTable[11] = {
    VOLTAGE_TO_ADC(4.2f / 2),  // 100
    VOLTAGE_TO_ADC(4.06f / 2), // 90
    VOLTAGE_TO_ADC(3.98f / 2), // 80
    VOLTAGE_TO_ADC(3.92f / 2), // 70
    VOLTAGE_TO_ADC(3.87f / 2), // 60
    VOLTAGE_TO_ADC(3.82f / 2), // 50
    VOLTAGE_TO_ADC(3.79f / 2), // 40
    VOLTAGE_TO_ADC(3.77f / 2), // 30
    VOLTAGE_TO_ADC(3.74f / 2), // 20
    VOLTAGE_TO_ADC(3.68f / 2), // 10
    VOLTAGE_TO_ADC(3.0f / 2),  // 0
};
static inline uint8_t BatAdcToPercent(int16_t adc)
{
    uint8_t offset = 0;
    for (uint8_t i = 0; i < CL_ARRAY_LENGTH(batAdcTable); i++)
    {
        offset = i;
        if (adc >= batAdcTable[i])
        {
            break;
        }
    }
    return 100 - offset * 10;
}

void BatMonitor_Process(void)
{
    switch (batStatus)
    {
    case BatSta_Ok:
        OkProc();
        break;
    case BatSta_Low:
        LowProc();
        break;
    case BatSta_Charge:
        ChargeProc();
        break;
    }

    static uint32_t lastTime = 0;
    if (SysTimeSpan(lastTime) >= 300)
    {
        lastTime = GetSysTime();

        extVolAdc = GetAdcResult(AdcChan_ExtVol);

        int16_t batAdc = GetAdcResult(AdcChan_Battery0);
        batPercent = BatAdcToPercent(batAdc);
    }
}

BatStatus_t GetBatStatus(void)
{
    return batStatus;
}

uint8_t GetBatPercent(void)
{
    return batPercent;
}
