#include "sdk_common.h"
#include "ble_cssc.h"
#include <string.h>
#include "ble_srv_common.h"
#include "nrf_log.h"
#include "stdio.h"

#define OPCODE_LENGTH 1                                                                  /**< Length of opcode inside Heart Rate Measurement packet. */
#define HANDLE_LENGTH 2                                                                  /**< Length of handle inside Heart Rate Measurement packet. */
#define DEF_MAX_COMM_LEN (NRF_SDH_BLE_GATT_MAX_MTU_SIZE - OPCODE_LENGTH - HANDLE_LENGTH) /**< Maximum size of a transmitted Heart Rate Measurement. */

/**@brief Function for handling the Connect event.
 *
 * @param[in]   p_cssc       Heart Rate Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_connect(ble_cssc_t *p_cssc, ble_evt_t const *p_ble_evt)
{
    p_cssc->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}

/**@brief Function for handling the Disconnect event.
 *
 * @param[in]   p_cssc       Heart Rate Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_disconnect(ble_cssc_t *p_cssc, ble_evt_t const *p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_cssc->conn_handle = BLE_CONN_HANDLE_INVALID;
}

/**@brief Function for handling write events to the Heart Rate Measurement characteristic.
 *
 * @param[in]   p_cssc         Heart Rate Service structure.
 * @param[in]   p_evt_write   Write event received from the BLE stack.
 */
static void on_notify_cccd_write(ble_cssc_t *p_cssc, ble_gatts_evt_write_t const *p_evt_write)
{
    if (p_evt_write->len == 2)
    {
        // CCCD written, update notification state
        ble_cssc_evt_arg_t evt_arg;
        if (ble_srv_is_notification_enabled(p_evt_write->data))
        {
            evt_arg.evt_type = BLE_CSSC_EVT_NOTIFICATION_ENABLED;
        }
        else
        {
            evt_arg.evt_type = BLE_CSSC_EVT_NOTIFICATION_DISABLED;
        }

        if (p_cssc->evt_handler != NULL)
        {
            p_cssc->evt_handler(p_cssc, &evt_arg);
        }
    }
}

/**@brief Function for handling the Write event.
 *
 * @param[in]   p_cssc       Heart Rate Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_write(ble_cssc_t *p_cssc, ble_evt_t const *p_ble_evt)
{
    ble_gatts_evt_write_t const *p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;
    if (p_evt_write->handle == p_cssc->notify_handles.cccd_handle)
    {
        on_notify_cccd_write(p_cssc, p_evt_write);
    }
    else if (p_evt_write->handle == p_cssc->write_handles.value_handle)
    {
        if (p_cssc->evt_handler != NULL)
        {
            ble_cssc_evt_arg_t evt_arg;
            evt_arg.evt_type = BLE_CSSC_EVT_WRITE;
            evt_arg.data = p_evt_write->data;
            evt_arg.length = p_evt_write->len;
            p_cssc->evt_handler(p_cssc, &evt_arg);
        }
    }
}

static void on_notify_done(ble_cssc_t *p_cssc, ble_evt_t const *p_ble_evt)
{
    if (p_cssc->evt_handler != NULL)
    {
        ble_cssc_evt_arg_t evt_arg;
        evt_arg.evt_type = BLE_CSSC_EVT_NOTIFY_DONE;
        evt_arg.data = NULL;
        evt_arg.length = 0;
        p_cssc->evt_handler(p_cssc, &evt_arg);
    }
}

void ble_cssc_on_ble_evt(ble_evt_t const *p_ble_evt, void *p_context)
{
    ble_cssc_t *p_cssc = (ble_cssc_t *)p_context;

    switch (p_ble_evt->header.evt_id)
    {
    case BLE_GAP_EVT_CONNECTED:
        on_connect(p_cssc, p_ble_evt);
        break;

    case BLE_GAP_EVT_DISCONNECTED:
        on_disconnect(p_cssc, p_ble_evt);
        break;

    case BLE_GATTS_EVT_WRITE:
        on_write(p_cssc, p_ble_evt);
        break;
    case BLE_GATTS_EVT_HVN_TX_COMPLETE:
        on_notify_done(p_cssc, p_ble_evt);
        break;
    default:
        // No implementation needed.
        break;
    }
}

uint32_t ble_cssc_init(ble_cssc_t *p_cssc, const ble_cssc_init_t *p_cssc_init)
{
    uint32_t err_code;
    ble_uuid_t ble_uuid;
    ble_add_char_params_t add_char_params;
    uint8_t init_notify_data[DEF_MAX_COMM_LEN];

    // Initialize service structure
    p_cssc->evt_handler = p_cssc_init->evt_handler;
    p_cssc->conn_handle = BLE_CONN_HANDLE_INVALID;
    p_cssc->max_comm_len = DEF_MAX_COMM_LEN;

    // Add service
    BLE_UUID_BLE_ASSIGN(ble_uuid, BLE_CSSC_SERVICE_ID);

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &ble_uuid,
                                        &p_cssc->service_handle);

    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Add notify characteristic
    memset(&add_char_params, 0, sizeof(add_char_params));

    add_char_params.uuid = BLE_CSSC_NOTIFY_ID;
    add_char_params.max_len = DEF_MAX_COMM_LEN;
    add_char_params.init_len = DEF_MAX_COMM_LEN;
    add_char_params.p_init_value = init_notify_data;
    add_char_params.is_var_len = true;
    add_char_params.char_props.notify = 1;
    add_char_params.cccd_write_access = SEC_OPEN;

    err_code = characteristic_add(p_cssc->service_handle, &add_char_params, &(p_cssc->notify_handles));
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Add write no response characteristic
    memset(&add_char_params, 0, sizeof(add_char_params));

    add_char_params.uuid = BLE_CSSC_WRITE_ID;
    add_char_params.max_len = DEF_MAX_COMM_LEN;
    add_char_params.init_len = DEF_MAX_COMM_LEN;
    add_char_params.char_props.write_wo_resp = 1;
    add_char_params.write_access = SEC_OPEN;

    err_code = characteristic_add(p_cssc->service_handle, &add_char_params, &(p_cssc->write_handles));
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    return NRF_SUCCESS;
}

uint32_t ble_cssc_send(ble_cssc_t *p_cssc, const uint8_t *data, const uint8_t length)
{
    uint32_t err_code;

    if (length > p_cssc->max_comm_len)
        return 0;
    // Send value if connected and notifying
    if (p_cssc->conn_handle != BLE_CONN_HANDLE_INVALID)
    {
        uint16_t hvx_len;
        ble_gatts_hvx_params_t hvx_params;

        memset(&hvx_params, 0, sizeof(hvx_params));
        hvx_len = length;

        hvx_params.handle = p_cssc->notify_handles.value_handle;
        hvx_params.type = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.offset = 0;
        hvx_params.p_len = &hvx_len;
        hvx_params.p_data = data;

        err_code = sd_ble_gatts_hvx(p_cssc->conn_handle, &hvx_params);
        if ((err_code == NRF_SUCCESS) && (hvx_len != length))
        {
            err_code = NRF_ERROR_DATA_SIZE;
        }
    }
    else
    {
        err_code = NRF_ERROR_INVALID_STATE;
    }

    return err_code;
}

void ble_cssc_on_gatt_evt(ble_cssc_t *p_cssc, nrf_ble_gatt_evt_t const *p_gatt_evt)
{
    if ((p_cssc->conn_handle == p_gatt_evt->conn_handle) && (p_gatt_evt->evt_id == NRF_BLE_GATT_EVT_ATT_MTU_UPDATED))
    {
        p_cssc->max_comm_len = p_gatt_evt->params.att_mtu_effective;
    }
}
