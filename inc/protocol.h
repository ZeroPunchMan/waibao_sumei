#pragma once

#include "cl_common.h"

typedef struct 
{
    uint8_t dir;
    uint8_t len;
    uint8_t type;
    uint8_t data[256];
} ProtoPack_t;

typedef CL_Result_t (*ProtoSendFunc)(const uint8_t *buff, uint16_t count);

void ProtocolInit(ProtoSendFunc);

void ProtocolRecvByte(uint8_t b);
void ProtocolSendPack(uint8_t len, uint8_t type, const uint8_t* data);

