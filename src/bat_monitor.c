#include "bat_monitor.h"
#include "ladc.h"
#include "nrf_gpio.h"
#include "cl_log.h"
#include "systime.h"
#include "sys_output.h"
#include "led.h"

#define EXT_VOL_THRESH (VOLTAGE_TO_ADC(0.8f))

static BatStatus_t batStatus = BatSta_Ok;
static int16_t extVolAdc = 0;

void BatMonitor_Init(void)
{
    nrf_gpio_cfg_input(NRF_GPIO_PIN_MAP(0, 25), NRF_GPIO_PIN_PULLUP); // 充电检测
}

static inline bool IsChargeFull(void)
{
    return nrf_gpio_pin_read(NRF_GPIO_PIN_MAP(0, 25)) == 0;
}

static void ToLow(void);
static void ToCharge(void);
static void ToOk(void)
{
    batStatus = BatSta_Ok;
    CL_LOG("bat ok");
}

static void OkProc(void)
{
    if (extVolAdc > EXT_VOL_THRESH)
    {
        ToCharge();
    }
    else if (GetBatPercent() <= 30)
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
        if (GetBatPercent() >= 40)
        {
            ToOk();
        }
    }
}

static void ToCharge(void)
{
    batStatus = BatSta_Charge;
    CL_LOG("bat charge");
    SysOutput_Stop();
}

static void ChargeProc(void)
{
    if (extVolAdc > EXT_VOL_THRESH)
    {
    }
    else
    {
        ToOk();
    }
}

int8_t batteryStage = 4;

typedef struct
{
    uint16_t minAdc, maxAdc;
} batStageDef_t;

static const batStageDef_t batStageDef[5] = {
    {.minAdc = VOLTAGE_TO_ADC(3.45f / 2), .maxAdc = VOLTAGE_TO_ADC(3.75f / 2)},  // 0
    {.minAdc = VOLTAGE_TO_ADC(3.72f / 2), .maxAdc = VOLTAGE_TO_ADC(3.81f / 2)},   // 1
    {.minAdc = VOLTAGE_TO_ADC(3.78f / 2), .maxAdc = VOLTAGE_TO_ADC(3.89f / 2)}, // 2
    {.minAdc = VOLTAGE_TO_ADC(3.86f / 2), .maxAdc = VOLTAGE_TO_ADC(4.02f / 2)},  // 3
    {.minAdc = VOLTAGE_TO_ADC(3.99f / 2), .maxAdc = VOLTAGE_TO_ADC(4.2f / 2)},   // 4
};

static void DoUpdateStage(uint16_t adcVal)
{
    const batStageDef_t *stageParam = batStageDef + batteryStage;
    if (adcVal > stageParam->maxAdc)
    {
        if (batteryStage < CL_ARRAY_LENGTH(batStageDef) - 1)
            batteryStage++;
    }
    else if (adcVal < stageParam->minAdc)
    {
        if (batteryStage > 0)
            batteryStage--;
    }
}

static void UpdateBatStage(uint16_t adcVal)
{
    static bool firstTime = true;

    if (firstTime)
    {
        for (int i = 0; i < CL_ARRAY_LENGTH(batStageDef) - 1; i++)
        {
            DoUpdateStage(adcVal);
        }
        firstTime = false;
    }
    else
    {
        DoUpdateStage(adcVal);
    }
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
    case BatSta_ChargeFull:
        ChargeProc();
        break;
    }

    static uint32_t lastTime = 0;
    if (SysTimeSpan(lastTime) >= 1500)
    {
        lastTime = GetSysTime();

        extVolAdc = GetAdcResult(AdcChan_ExtVol);

        int16_t batAdc = GetBatteryAdc();
        // todo 减去充电压差
        UpdateBatStage(batAdc); // todo review
    }
}

BatStatus_t GetBatStatus(void)
{
    if (batStatus == BatSta_Charge && IsChargeFull())
    {
        return BatSta_ChargeFull;
    }
    return batStatus;
}

uint8_t GetBatPercent(void)
{
    return (batteryStage + 1) * 20;
}
