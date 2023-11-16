#include "cl_common.h"
#include "cl_event_system.h"
#include "protocol.h"
#include "systime.h"

typedef enum
{
    PS_Head,
    PS_Dir,
    PS_Token,
    PS_Len,
    PS_Type,
    PS_Data,
    PS_Verify,
} ParseStatus_t;

typedef struct
{
    ProtoPack_t recvPacket;
    ParseStatus_t parseStatus;
    uint8_t receivedLen;
} ParseContext_t;
static ParseContext_t context = {.parseStatus = PS_Head, .receivedLen = 0};

uint8_t CalcVerify(const ProtoPack_t *pack)
{
    uint8_t sum = 0;
    sum += pack->type;
    for (uint8_t i = 0; i < pack->len - 1; i++)
    {
        sum += pack->data[i];
    }
    return sum;
}

void ProtocolRecvByte(uint8_t b)
{
    static uint32_t lastRecvTime = 0;
    if (SysTimeSpan(lastRecvTime) >= 500)
    {
        context.parseStatus = PS_Head;
    }

    lastRecvTime = GetSysTime();

    switch (context.parseStatus)
    {
    case PS_Head:
        if (b == 0xae)
            context.parseStatus = PS_Dir;
        break;

    case PS_Dir:
        if (b == 0x01 || b == 0x02)
        {
            context.recvPacket.dir = b;
            context.parseStatus = PS_Token;
        }
        else
        {
            context.parseStatus = PS_Head;
        }
        break;

    case PS_Token:
        if (b == 0xf0)
            context.parseStatus = PS_Len;
        else
            context.parseStatus = PS_Head;
        break;

    case PS_Len:
        context.recvPacket.len = b;
        context.parseStatus = PS_Type;
        break;

    case PS_Type:
        context.recvPacket.type = b;
        context.receivedLen = 0;
        if (b > 1)
            context.parseStatus = PS_Data;
        else
            context.parseStatus = PS_Verify;

        break;

    case PS_Data:
        context.recvPacket.data[context.receivedLen++] = b;
        if (context.receivedLen >= context.recvPacket.len)
            context.parseStatus = PS_Verify;
        break;

    case PS_Verify:
        if (b == CalcVerify(&context.recvPacket))
        { // todo raise event
            CL_EventSysRaise(CL_Event_RecvMsg, 0, &context.recvPacket);
        }

        context.parseStatus = PS_Head;
        break;
    }
}

static ProtoSendFunc sendFunc = CL_NULL;
void ProtocolSendPack(uint8_t len, uint8_t type, const uint8_t *data)
{
    static uint8_t buff[10];
    buff[0] = 0xae;
    buff[1] = 0x02;
    buff[2] = 0xf0;
    buff[3] = len;
    buff[4] = type;

    uint8_t sum = type;
    sendFunc(buff, 5);

    if (len > 1)
    {
        for (uint8_t i = 0; i < len - 1; i++)
        {
            sum += data[i];
        }
        sendFunc(data, len - 1);
    }
    
    sendFunc(&sum, 1);
}

void ProtocolInit(ProtoSendFunc s)
{
    sendFunc = s;
}
