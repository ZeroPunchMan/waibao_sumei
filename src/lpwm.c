#include "lpwm.h"
#include "nrf_drv_pwm.h"
#include "nrf_drv_clock.h"

//--------------------pwm------------------------------
static nrf_drv_pwm_t m_pwm0 = NRF_DRV_PWM_INSTANCE(0);
void MosPwmInit(void)
{
    nrf_drv_pwm_config_t const config0 =
        {
            .output_pins =
                {
                    15, // channel 0
                    16, // channel 1
                    17, // channel 2
                    19  // channel 3
                },
            .irq_priority = APP_IRQ_PRIORITY_LOWEST,
            .base_clock = NRF_PWM_CLK_125kHz,
            .count_mode = NRF_PWM_MODE_UP,
            .top_value = 15625,
            .load_mode = NRF_PWM_LOAD_INDIVIDUAL,
            .step_mode = NRF_PWM_STEP_AUTO};
    APP_ERROR_CHECK(nrf_drv_pwm_init(&m_pwm0, &config0, NULL));

    // This array cannot be allocated on stack (hence "static") and it must
    // be in RAM (hence no "const", though its content is not changed).
    static nrf_pwm_values_individual_t /*const*/ seq_values[] =
        {
            {0x8000, 0, 0, 0},
            {0, 0x8000, 0, 0},
            {0, 0, 0x8000, 0},
            {0, 0, 0, 0x8000}};
    nrf_pwm_sequence_t const seq =
        {
            .values.p_individual = seq_values,
            .length = NRF_PWM_VALUES_LENGTH(seq_values),
            .repeats = 0,
            .end_delay = 0};

    (void)nrf_drv_pwm_simple_playback(&m_pwm0, &seq, 1, NRF_DRV_PWM_FLAG_LOOP);
}

//------------------tim + gpiote---------------------
void LedPwmInit(void)
{
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
        break;
    case PwmChan_Mos1:
        break;
    case PwmChan_Mos2:
        break;
    case PwmChan_Mos3:
        break;
    case PwmChan_Led0:
        break;
    case PwmChan_Led1:
        break;
    case PwmChan_Led2:
        break;
    }
}
