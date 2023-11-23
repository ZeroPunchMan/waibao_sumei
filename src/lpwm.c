#include "lpwm.h"
#include "nrf_drv_pwm.h"
#include "nrf_drv_clock.h"
#include "nrf_drv_timer.h"
#include "cl_log.h"
#include "breath_rgb.h"

//--------------------pwm------------------------------
static nrf_drv_pwm_t m_pwm0 = NRF_DRV_PWM_INSTANCE(0);
static nrf_pwm_values_individual_t /*const*/ seq_values[] = {
    {100, 100, 100, 100},
};
void MosPwmInit(void)
{
    nrf_drv_pwm_config_t const config0 =
        {
            .output_pins =
                {
                    19, // channel 0
                    15, // channel 1
                    16, // channel 2
                    17  // channel 3
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

    Pwm_SetOutput(PwmChan_Mos0, 0);
    Pwm_SetOutput(PwmChan_Mos1, 0);
    Pwm_SetOutput(PwmChan_Mos2, 0);
    Pwm_SetOutput(PwmChan_Mos3, 0);
}

//------------------timer---------------------
void timer_led_event_handler(nrf_timer_event_t event_type, void* p_context)
{
    BreathRgb_Update();
}

const nrf_drv_timer_t TIMER_LED = NRF_DRV_TIMER_INSTANCE(2);
void LedPwmInit(void)
{
    uint32_t time_ticks;
    uint32_t err_code = NRF_SUCCESS;

    //Configure TIMER_LED for generating simple light effect - leds on board will invert his state one after the other.
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
    MosPwmInit();
    LedPwmInit();
}

void Pwm_SetOutput(PwmChannel_t chan, uint8_t duty)
{
    switch (chan)
    {
    case PwmChan_Mos0:
        seq_values->channel_0 = 100 - duty;
        break;
    case PwmChan_Mos1:
        seq_values->channel_1 = 100 - duty;
        break;
    case PwmChan_Mos2:
        seq_values->channel_2 = 100 - duty;
        break;
    case PwmChan_Mos3:
        seq_values->channel_3 = 100 - duty;
        break;
    }
}
