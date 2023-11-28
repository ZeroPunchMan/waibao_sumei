#include "lpwm.h"
#include "nrf_drv_pwm.h"
#include "nrf_drv_clock.h"
#include "nrf_drv_timer.h"
#include "cl_log.h"
#include "breath_rgb.h"
#include "nrf_gpio.h"

//--------------------pwm------------------------------
typedef struct
{
    uint8_t freq;
    uint8_t power;
    uint32_t pin;

    uint16_t count;
    bool work;
} MosChanContext_t;

static const uint32_t freqTable[5] = {1667, 1429, 1250, 1111, 1000};

volatile static MosChanContext_t mosChanCtx[4] =
    {
        [0] = {.freq = 0, .power = 0, .pin = NRF_GPIO_PIN_MAP(0, 19), .count = 0, .work = false},
        [1] = {.freq = 0, .power = 0, .pin = NRF_GPIO_PIN_MAP(0, 15), .count = 0, .work = false},
        [2] = {.freq = 0, .power = 0, .pin = NRF_GPIO_PIN_MAP(0, 16), .count = 0, .work = false},
        [3] = {.freq = 0, .power = 0, .pin = NRF_GPIO_PIN_MAP(0, 17), .count = 0, .work = false},
};
volatile static bool mosChanRun = false;

static inline uint32_t FreqToCount(uint8_t freq)
{
    return freqTable[freq];
}

static inline uint32_t PowerToCount(uint8_t power)
{
    return power / 10;
}

static void MosChanInit(void)
{
    for (int i = 0; i < CL_ARRAY_LENGTH(mosChanCtx); i++)
    {
        nrf_gpio_cfg_output(mosChanCtx[i].pin);
        nrf_gpio_pin_clear(mosChanCtx[i].pin);
    }
}

static void MosChanUpdate(volatile MosChanContext_t *ctx)
{
    if (!mosChanRun)
    {
        nrf_gpio_pin_clear(ctx->freq);
        return;
    }

    ctx->count++;
    if (ctx->work)
    {
        if (ctx->count >= PowerToCount(ctx->power))
        {
            ctx->work = false;
            ctx->count = 0;
            nrf_gpio_pin_clear(ctx->pin);
        }
    }
    else
    {
        if (ctx->count >= FreqToCount(ctx->freq))
        {
            ctx->work = true;
            ctx->count = 0;
            nrf_gpio_pin_set(ctx->pin);
        }
    }
}

void MosChanSet(uint8_t chan, uint8_t power, uint8_t freq)
{
    mosChanCtx[chan].power = power;
    mosChanCtx[chan].freq = freq - 6;
}

void MosRunPause(bool run)
{
    mosChanRun = run;
}

//------------------timer---------------------
void timer_led_event_handler(nrf_timer_event_t event_type, void *p_context)
{
    BreathRgb_Update();
    MosChanUpdate(mosChanCtx);
    MosChanUpdate(mosChanCtx + 1);
    MosChanUpdate(mosChanCtx + 2);
    MosChanUpdate(mosChanCtx + 3);

    uint32_t sumOfPwr = 0;
    for (int i = 0; i < CL_ARRAY_LENGTH(mosChanCtx); i++)
        sumOfPwr += mosChanCtx[i].power;

    sumOfPwr /= 80;
    sumOfPwr = CL_MIN(sumOfPwr, BLF_Freq4);
    BreatRgb_SetFreq(sumOfPwr, sumOfPwr, sumOfPwr);
}

const nrf_drv_timer_t TIMER_LED = NRF_DRV_TIMER_INSTANCE(2);
void LedPwmInit(void)
{
    uint32_t time_ticks;
    uint32_t err_code = NRF_SUCCESS;

    // Configure TIMER_LED for generating simple light effect - leds on board will invert his state one after the other.
    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    err_code = nrf_drv_timer_init(&TIMER_LED, &timer_cfg, timer_led_event_handler);
    APP_ERROR_CHECK(err_code);

    time_ticks = nrf_drv_timer_us_to_ticks(&TIMER_LED, 100);
    CL_LOG("ticks: %d", time_ticks);
    nrf_drv_timer_extended_compare(
        &TIMER_LED, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);

    nrf_drv_timer_enable(&TIMER_LED);
}

void Pwm_Init(void)
{
    MosChanInit();
    LedPwmInit();
}
