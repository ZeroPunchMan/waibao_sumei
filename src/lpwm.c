#include "lpwm.h"
#include "nrf_drv_pwm.h"
#include "nrf_drv_clock.h"
#include "nrf_drv_timer.h"
#include "cl_log.h"
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

void timer_led_event_handler(nrf_timer_event_t event_type, void *p_context);

static const uint32_t freqTable[5] = {1667, 1429, 1250, 1111, 1000};

volatile static MosChanContext_t mosChanCtx[4] = {
    [0] = {.freq = 0, .power = 0, .pin = NRF_GPIO_PIN_MAP(0, 12), .count = 0, .work = false},
    [1] = {.freq = 0, .power = 0, .pin = NRF_GPIO_PIN_MAP(0, 14), .count = 0, .work = false},
    [2] = {.freq = 0, .power = 0, .pin = NRF_GPIO_PIN_MAP(0, 16), .count = 0, .work = false},
    [3] = {.freq = 0, .power = 0, .pin = NRF_GPIO_PIN_MAP(0, 15), .count = 0, .work = false},
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

const nrf_drv_timer_t TIMER_LED = NRF_DRV_TIMER_INSTANCE(2);
static void MosChanInit(void)
{
    for (int i = 0; i < CL_ARRAY_LENGTH(mosChanCtx); i++)
    {
        nrf_gpio_cfg_output(mosChanCtx[i].pin);
        nrf_gpio_pin_clear(mosChanCtx[i].pin);
    }

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

static void MosChanUpdate(volatile MosChanContext_t *ctx)
{
    if (!mosChanRun || !ctx->power)
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
    MosChanUpdate(mosChanCtx);
    MosChanUpdate(mosChanCtx + 1);
    MosChanUpdate(mosChanCtx + 2);
    MosChanUpdate(mosChanCtx + 3);
}

static nrf_drv_pwm_t m_pwm0 = NRF_DRV_PWM_INSTANCE(0);
static nrf_pwm_values_individual_t seq_values[] = {
    {100, 100, 100, 100},
};
void LedPwmInit(void)
{
    nrf_drv_pwm_config_t const config0 =
        {
            .output_pins =
                {
                    26, // channel 0
                    27, // channel 1
                    9,  // channel 2
                    10  // channel 3
                },
            .irq_priority = APP_IRQ_PRIORITY_LOWEST,
            .base_clock = NRF_PWM_CLK_1MHz,
            .count_mode = NRF_PWM_MODE_UP,
            .top_value = 100,
            .load_mode = NRF_PWM_LOAD_INDIVIDUAL,
            .step_mode = NRF_PWM_STEP_AUTO};
    APP_ERROR_CHECK(nrf_drv_pwm_init(&m_pwm0, &config0, NULL));

    // This array cannot be allocated on stack (hence "static") and it must
    // be in RAM (hence no "const", though its content is not changed).
    nrf_pwm_sequence_t const seq = {
        .values.p_individual = seq_values,
        .length = NRF_PWM_VALUES_LENGTH(seq_values),
        .repeats = 0,
        .end_delay = 0,
    };

    (void)nrf_drv_pwm_simple_playback(&m_pwm0, &seq, 1, NRF_DRV_PWM_FLAG_LOOP);

    SetPwmChan(PwmChan_Led0, 0);
    SetPwmChan(PwmChan_Led1, 0);
}

void Pwm_Init(void)
{
    MosChanInit();
    LedPwmInit();
}

void SetPwmChan(PwmChannel_t chan, uint8_t duty)
{
    switch (chan)
    {
    case PwmChan_Led0:
        seq_values->channel_0 = 100 - duty;
        break;
    case PwmChan_Led1:
        seq_values->channel_1 = 100 - duty;
        break;
    }
}
