#ifndef BLE_CSSC_H_
#define BLE_CSSC_H_

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"
#include "nrf_sdh_ble.h"
#include "nrf_ble_gatt.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define BLE_CSSC_SERVICE_ID 0xffe0
#define BLE_CSSC_NOTIFY_ID 0xffe1
#define BLE_CSSC_WRITE_ID 0xffe2

#define BL_CSSC_DEF(_name)   \
    static ble_cssc_t _name; \
    NRF_SDH_BLE_OBSERVER(_name##_obs, 2, ble_cssc_on_ble_evt, &_name)

    // Forward declaration of the ble_cssc_t type.
    typedef enum
    {
        BLE_CSSC_EVT_NOTIFICATION_ENABLED,  /**< Heart Rate value notification enabled event. */
        BLE_CSSC_EVT_NOTIFICATION_DISABLED, /**< Heart Rate value notification disabled event. */
        BLE_CSSC_EVT_WRITE,
        BLE_CSSC_EVT_NOTIFY_DONE,
    } ble_cssc_evt_type_t;

    /**@brief Heart Rate Service event. */
    typedef struct
    {
        ble_cssc_evt_type_t evt_type; /**< Type of event. */
        const uint8_t *data;
        uint8_t length;
    } ble_cssc_evt_arg_t;
    typedef struct ble_cssc_s ble_cssc_t;

    /**@brief Heart Rate Service structure. This contains various status information for the service. */
    typedef void (*ble_cssc_evt_handler_t)(ble_cssc_t *p_cssc, ble_cssc_evt_arg_t *p_evt);
    typedef struct
    {
        ble_cssc_evt_handler_t evt_handler; /**< Event handler to be called for handling events in the Heart Rate Service. */
    } ble_cssc_init_t;
    struct ble_cssc_s
    {
        ble_cssc_evt_handler_t evt_handler;      /**< Event handler to be called for handling events in the Heart Rate Service. */
        uint16_t service_handle;                 /**< Handle of Heart Rate Service (as provided by the BLE stack). */
        ble_gatts_char_handles_t notify_handles; /**< Handles related to the Heart Rate Measurement characteristic. */
        ble_gatts_char_handles_t write_handles;  /**< Handles related to the Body Sensor Location characteristic. */
        uint16_t conn_handle;                    /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection). */
        uint8_t max_comm_len;                    /**< Current maximum HR measurement length, adjusted according to the current ATT MTU. */
    };

    uint32_t ble_cssc_init(ble_cssc_t *p_cssc, ble_cssc_init_t const *p_cssc_init);

    void ble_cssc_on_gatt_evt(ble_cssc_t *p_cssc, nrf_ble_gatt_evt_t const *p_gatt_evt);

    /**@brief Function for handling the Application's BLE Stack events.
 *
 * @details Handles all events from the BLE stack of interest to the Heart Rate Service.
 *
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 * @param[in]   p_context   Heart Rate Service structure.
 */
    void ble_cssc_on_ble_evt(ble_evt_t const *p_ble_evt, void *p_context);

    uint32_t ble_cssc_send(ble_cssc_t *p_cssc, const uint8_t *data, const uint8_t length);

#ifdef __cplusplus
}
#endif

#endif // BLE_CSSC_H_

/** @} */
