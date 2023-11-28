#include "sgp_ble_agent.h"
#include "cl_queue.h"
#include "nrf_log.h"
#include "protocol.h"

extern uint32_t SendWithNotify(const uint8_t *buff, const uint8_t count);

static bool connected = false;

CL_QUEUE_DEF_INIT(sgpRecv_q, 128, uint8_t, static);
CL_QUEUE_DEF_INIT(sgpSend_q, 256, uint8_t, static);

static CL_Result_t SGP_BleSend(const uint8_t *buff, uint16_t count)
{
    if (!connected)
        return CL_ResFailed;

    if (CL_QueueFreeSpace(&sgpSend_q) < count)
        return CL_ResFailed;

    for (uint16_t i = 0; i < count; i++)
    {
        CL_QueueAdd(&sgpSend_q, (void *)(buff + i));
    }

    return CL_ResSuccess;
}

void SgpBleAgent_Init(void)
{
    ProtocolInit(SGP_BleSend);
}

void SgpBleAgent_Receive(const uint8_t *data, uint8_t length)
{
    if (CL_QueueFreeSpace(&sgpRecv_q) < length)
        return;

    for (uint8_t i = 0; i < length; i++)
    {
        CL_QueueAdd(&sgpRecv_q, (void *)(data + i));
    }
}

void SgpBleAgent_ChangeConnStatus(bool isConnected)
{
    NRF_LOG_INFO("sgp connected: %d", isConnected);
    connected = isConnected;
}

static uint8_t sendBuff[50];
static uint8_t sendCount = 0;

void SgpBleAgent_Process(void)
{
    //---------------recv process-----------------
    uint8_t data;
    for (uint8_t i = 0; i < 50; i++) //50 times max
    {
        if (CL_QueuePoll(&sgpRecv_q, &data) == CL_ResSuccess)
        {
            //receive one byte
            ProtocolRecvByte(data);
        }
        else
        {
            break;
        }
    }

    //----------------send process------------------------
    if (!connected)
    {
        CL_QueueClear(&sgpSend_q);
        sendCount = 0;
        return;
    }

    for (uint8_t tryTimes = 0; tryTimes < 7; tryTimes++)
    {
        if (sendCount != 0)
        {
            if (NRF_SUCCESS == SendWithNotify(sendBuff, sendCount))
            {
                NRF_LOG_INFO("send %d bytes", sendCount);
                sendCount = 0;
            }
            else
            {
                break;
            }
        }

        if (!CL_QueueEmpty(&sgpSend_q))
        {
            for (int i = 0; i < CL_ARRAY_LENGTH(sendBuff); i++)
            {
                uint8_t temp;
                if (CL_QueuePoll(&sgpSend_q, &temp) != CL_ResSuccess)
                    break;

                sendBuff[sendCount++] = temp;
            }
        }
        else
        {
            break;
        }
    }
}
