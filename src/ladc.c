
#include "nrf.h"
#include "nrf_drv_saadc.h"
#include "nrf_log.h"
#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"
#include "ladc.h"
#include "systime.h"
#include "cl_log.h"

static nrf_saadc_value_t adcResult[4];
int16_t GetAdcResult(AdcChannel_t chan)
{
    return adcResult[chan];
}

// ##########################ADC driver###########################################
#define ADC_BUFF_SIZE (4)
static const nrf_drv_timer_t m_timer = NRF_DRV_TIMER_INSTANCE(1);
static nrf_saadc_value_t m_buffer_pool[2][ADC_BUFF_SIZE];
static nrf_ppi_channel_t m_ppi_channel;

void saadc_callback(nrf_drv_saadc_evt_t const *p_event)
{
    if (p_event->type == NRF_DRV_SAADC_EVT_DONE)
    {
        ret_code_t err_code;

        err_code = nrf_drv_saadc_buffer_convert(p_event->data.done.p_buffer, ADC_BUFF_SIZE);
        APP_ERROR_CHECK(err_code);

        adcResult[AdcChan_Current] = p_event->data.done.p_buffer[0];
        adcResult[AdcChan_ExtVol] = p_event->data.done.p_buffer[1];
        adcResult[AdcChan_Battery0] = p_event->data.done.p_buffer[2];
        adcResult[AdcChan_Battery1] = p_event->data.done.p_buffer[3];
    }
}

void saadc_init(void)
{
    ret_code_t err_code;

    err_code = nrf_drv_saadc_init(NULL, saadc_callback);
    APP_ERROR_CHECK(err_code);

    // 充电电流
    nrf_saadc_channel_config_t channel_config = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN3);
    err_code = nrf_drv_saadc_channel_init(0, &channel_config);
    APP_ERROR_CHECK(err_code);

    // 外部电压
    nrf_saadc_channel_config_t battery_config = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN7);
    err_code = nrf_drv_saadc_channel_init(1, &battery_config);
    APP_ERROR_CHECK(err_code);

    // 电池电压
    nrf_saadc_channel_config_t ntc_config = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN4);
    err_code = nrf_drv_saadc_channel_init(2, &ntc_config);
    APP_ERROR_CHECK(err_code);

    // 电池电压2
    nrf_saadc_channel_config_t breath_config = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN5);
    err_code = nrf_drv_saadc_channel_init(3, &breath_config);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_buffer_convert(m_buffer_pool[0], ADC_BUFF_SIZE);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_buffer_convert(m_buffer_pool[1], ADC_BUFF_SIZE);
    APP_ERROR_CHECK(err_code);
}

void timer_handler(nrf_timer_event_t event_type, void *p_context)
{
}

void saadc_sampling_event_init(void)
{
    ret_code_t err_code;
    err_code = nrf_drv_ppi_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_timer_config_t timer_config = NRF_DRV_TIMER_DEFAULT_CONFIG;
    timer_config.frequency = NRF_TIMER_FREQ_4MHz;
    timer_config.bit_width = NRF_TIMER_BIT_WIDTH_32;
    err_code = nrf_drv_timer_init(&m_timer, &timer_config, timer_handler);
    APP_ERROR_CHECK(err_code);

    /* setup m_timer for compare event */
    uint32_t ticks = nrf_drv_timer_ms_to_ticks(&m_timer, 1000 / 500);
    nrf_drv_timer_extended_compare(&m_timer, NRF_TIMER_CC_CHANNEL0, ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, false);
    nrf_drv_timer_enable(&m_timer);

    uint32_t timer_compare_event_addr = nrf_drv_timer_compare_event_address_get(&m_timer, NRF_TIMER_CC_CHANNEL0);
    uint32_t saadc_sample_event_addr = nrf_drv_saadc_sample_task_get();

    /* setup ppi channel so that timer compare event is triggering sample task in SAADC */
    err_code = nrf_drv_ppi_channel_alloc(&m_ppi_channel);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_ppi_channel_assign(m_ppi_channel, timer_compare_event_addr, saadc_sample_event_addr);
    APP_ERROR_CHECK(err_code);
}

void saadc_sampling_event_enable(void)
{
    ret_code_t err_code = nrf_drv_ppi_channel_enable(m_ppi_channel);

    APP_ERROR_CHECK(err_code);
}

void Adc_Init(void)
{
    saadc_init();
    saadc_sampling_event_init();
    saadc_sampling_event_enable();
}

static int16_t batAdcLastSec = 0;
static void BatAdcFilter(void)
{
    static uint32_t batAdcTime = 0;
    static int16_t batMaxAdc = INT16_MIN;
    if (SysTimeSpan(batAdcTime) < 300)
    {
        int16_t curAdc = GetAdcResult(AdcChan_Battery1);
        if (curAdc > batMaxAdc)
            batMaxAdc = curAdc;
    }
    else
    {
        batAdcLastSec = batMaxAdc;
        batMaxAdc = INT16_MIN;
        batAdcTime = GetSysTime();
    }
}

static int16_t chgAdcMin = INT16_MAX;
static int16_t chgAdcMax = INT16_MIN;
static void ChargeAdcFilter(void)
{
    static uint32_t windowTime = 0;
    if(SysTimeSpan(windowTime) < 1000)
    {
        int16_t curAdc = GetAdcResult(AdcChan_Current);
        if(curAdc > chgAdcMax)
            chgAdcMax = curAdc;
        
        if(curAdc < chgAdcMin)
            chgAdcMin = curAdc;
    }
    else
    {
        CL_LOG("chg: %d, %d", chgAdcMin, chgAdcMax);
        chgAdcMin = INT16_MAX;
        chgAdcMax = INT16_MIN;
        windowTime = GetSysTime();
    }
}

void Adc_Process(void)
{
    BatAdcFilter();
    ChargeAdcFilter();
}

int16_t GetBatteryAdc(void)
{
    return batAdcLastSec;
}
