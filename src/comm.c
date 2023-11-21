#include "comm.h"
#include "protocol.h"
#include "cl_event_system.h"
#include "cl_log.h"
#include "sgp_ble_agent.h"

typedef enum
{
    PT_Start = 0x01,
    PT_Pause = 0x02,
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

// static void SendBattery(uint8_t percent)
// {
//     ProtocolSendPack(2, PT_Battery, &percent);
// }

static void OnRecvStart(ProtoPack_t *pack)
{
    CL_LOG("recv start");
    //todo
    ProtocolSendPack(1, PT_Start, CL_NULL);
}

static void OnRecvPause(ProtoPack_t *pack)
{
    CL_LOG("recv pause");
    //todo
    ProtocolSendPack(1, PT_Pause, CL_NULL);
}

static void OnRecvStop(ProtoPack_t *pack)
{
    CL_LOG("recv stop");
    //todo
    ProtocolSendPack(1, PT_Stop, CL_NULL);
}

static void OnRecvMotors(ProtoPack_t *pack)
{
    CL_LOG("recv motors");
}

static bool OnRecvAppMsg(void *eventArg)
{
    ProtoPack_t *pack = (ProtoPack_t *)eventArg;
    if (pack->dir != 1)
        return false;

    switch (pack->type)
    {
    case PT_Start:
        OnRecvStart(pack);
        break;
    case PT_Pause:
        OnRecvPause(pack);
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
}


