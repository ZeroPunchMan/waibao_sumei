#include "comm.h"
#include "protocol.h"
#include "cl_event_system.h"
#include "cl_log.h"
#include "sgp_ble_agent.h"
#include "sys_output.h"
#include "bat_monitor.h"

typedef enum
{
    PT_RunPause = 0x01,
    PT_Stop = 0x03,
    PT_Motors = 0x04,
    PT_Battery = 0x05,
    PT_Version = 0x66,
} PackType_t;

static void SendVersion(void)
{
    uint8_t version[2] = {0, 1};
    CL_LOG("send version");
    ProtocolSendPack(3, PT_Version, version);
}

static void SendBattery(uint8_t percent, bool charge)
{
    uint8_t data[2] = {percent, charge ? 1 : 0};
    ProtocolSendPack(3, PT_Battery, data);
}

static void OnRecvStart(ProtoPack_t *pack)
{
    CL_LOG("recv runpause: %d", pack->data[0]);
    SysOutput_RunPause(pack->data[0] == 1);
    ProtocolSendPack(2, PT_RunPause, pack->data);
}

static void OnRecvStop(ProtoPack_t *pack)
{
    CL_LOG("recv stop");
    SysOutput_Stop();
    ProtocolSendPack(1, PT_Stop, CL_NULL);
}

static void OnRecvMotors(ProtoPack_t *pack)
{
    if (pack->len != 9)
        return;

    SysOutput_SetChannel(0, pack->data[0], pack->data[1]);
    SysOutput_SetChannel(1, pack->data[2], pack->data[3]);
    SysOutput_SetChannel(2, pack->data[4], pack->data[5]);
    SysOutput_SetChannel(3, pack->data[6], pack->data[7]);

    ProtocolSendPack(9, PT_Motors, pack->data);
    // CL_LOG("recv motors");
}

static bool OnRecvAppMsg(void *eventArg)
{
    ProtoPack_t *pack = (ProtoPack_t *)eventArg;
    if (pack->dir != 1)
        return false;

    switch (pack->type)
    {
    case PT_RunPause:
        OnRecvStart(pack);
        break;
    case PT_Stop:
        OnRecvStop(pack);
        break;
    case PT_Motors:
        OnRecvMotors(pack);
        break;
    case PT_Version:
        SendVersion();
        break;
    }
    return true;
}

void Comm_Init(void)
{
    SgpBleAgent_Init();

    CL_EventSysAddListener(OnRecvAppMsg, CL_Event_RecvPack, 0);
}

void Comm_Process(void)
{
    SgpBleAgent_Process();

    static uint32_t lastTime = 0;
    if (SysTimeSpan(lastTime) >= SYSTIME_SECOND(2))
    {
        lastTime = GetSysTime();

        SendBattery(GetBatPercent(), GetBatStatus() == BatSta_Charge);
    }
}
