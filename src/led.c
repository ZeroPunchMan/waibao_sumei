#include "led.h"
#include "systime.h"
#include "nrf_gpio.h"
#include "lpwm.h"
#include "bat_monitor.h"
#include "sys_output.h"

#define BLE_LED_PIN (NRF_GPIO_PIN_MAP(0, 20))

#define IND_LED1_PIN (NRF_GPIO_PIN_MAP(0, 17))
#define IND_LED2_PIN (NRF_GPIO_PIN_MAP(0, 18))

void Led_Init(void)
{
    nrf_gpio_cfg_output(BLE_LED_PIN); // 蓝牙指示灯

    nrf_gpio_cfg_output(IND_LED1_PIN); // 充电指示灯1
    nrf_gpio_cfg_output(IND_LED2_PIN); // 充电指示灯2
}

static BleLedStyle_t bleLedStyle = BleLedStyle_Blink;
void SetBleLed(BleLedStyle_t s)
{
    bleLedStyle = s;
}

static void BleLedProc(void)
{
    if (BleLedStyle_Blink == bleLedStyle)
    {
        static uint32_t lastTime = 0;
        if (SysTimeSpan(lastTime) >= 500)
        {
            lastTime = GetSysTime();
            nrf_gpio_pin_toggle(BLE_LED_PIN);
        }
    }
    else if (BleLedStyle_On == bleLedStyle)
    {
        nrf_gpio_pin_set(BLE_LED_PIN);
    }
}

static ChargeLedStyle_t chargeLedStyle = ChgLedStyle_None;
void SetChargeLed(ChargeLedStyle_t s)
{
    chargeLedStyle = s;
}
static void ChargeLedProc(void)
{
    if (chargeLedStyle == ChgLedStyle_None)
    {
        nrf_gpio_pin_clear(IND_LED1_PIN);
        nrf_gpio_pin_clear(IND_LED2_PIN);
    }
    else if (chargeLedStyle == ChgLedStyle_Charging)
    {
        nrf_gpio_pin_set(IND_LED1_PIN);
        nrf_gpio_pin_clear(IND_LED2_PIN);
    }
    else if (chargeLedStyle == ChgLedStyle_ChargeDone)
    {
        nrf_gpio_pin_clear(IND_LED1_PIN);
        nrf_gpio_pin_set(IND_LED2_PIN);
    }
    else if (chargeLedStyle == ChgLedStyle_Work)
    {
        static uint32_t lastTime = 0;
        static bool onOff = false;
        if (SysTimeSpan(lastTime) >= 500)
        {
            lastTime = GetSysTime();

            onOff = !onOff;
            if (onOff)
            {
                nrf_gpio_pin_clear(IND_LED1_PIN);
                nrf_gpio_pin_set(IND_LED2_PIN);
            }
            else
            {
                nrf_gpio_pin_set(IND_LED1_PIN);
                nrf_gpio_pin_clear(IND_LED2_PIN);
            }
        }
    }

    //todo 根据状态来设置
    BatStatus_t batSta = GetBatStatus();
    uint32_t sysSta = GetSysOutput();
    if(sysSta)
    {
        SetChargeLed(ChgLedStyle_Work);
    }
    else
    {
        switch (batSta)
        {
        case BatSta_Ok:
        case BatSta_Low:
            SetChargeLed(ChgLedStyle_None);
            break;
        case BatSta_Charge:
            SetChargeLed(ChgLedStyle_Charging);
            break;
        case BatSta_ChargeFull:
            SetChargeLed(ChgLedStyle_ChargeDone);
            break;
        }
    }
}

static BreathLedStyle_t breathLedStyle = BreathLed_Slow;
void SetBreathLed(BreathLedStyle_t s)
{
    breathLedStyle = s;
}
static volatile uint8_t breathLevel[3] = {0, 0, 0};
static volatile uint8_t breathDir[3] = {1, 1, 1};
static void BreathLedProc(void)
{
    static uint32_t levelTime = 0;
    if (SysTimeSpan(levelTime) >= breathLedStyle)
    {
        levelTime = GetSysTime();

        for (int i = 0; i < 2; i++)
        {
            if (breathLevel[i] >= 100)
                breathDir[i] = 0;
            else if (breathLevel[i] == 0)
                breathDir[i] = 1;

            if (breathDir[i])
                breathLevel[i]++;
            else
                breathLevel[i]--;
        }
    }

    SetPwmChan(PwmChan_Led0, breathLevel[0]);
    SetPwmChan(PwmChan_Led1, breathLevel[1]);
}

void Led_Process(void)
{
    BleLedProc();
    ChargeLedProc();
    BreathLedProc();
}
