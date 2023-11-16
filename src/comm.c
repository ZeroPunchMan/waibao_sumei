#include "comm.h"
#include "protocol.h"
#include "cl_event_system.h"
#include "cl_log.h"
#include "sgp_ble_agent.h"

typedef enum
{
    PT_Version = 0x66,
} PackType_t;

void SendVersion(void)
{
    uint8_t version[2] = {0, 1};
    CL_LOG("send version");
    ProtocolSendPack(3, PT_Version, version);
}

static bool OnRecvAppMsg(void *eventArg)
{
    const ProtoPack_t *pack = (const ProtoPack_t *)eventArg;
    if(pack->dir != 1)
        return false;

    switch (pack->type)
    {
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
