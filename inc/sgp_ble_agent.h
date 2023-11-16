#pragma once

#include "cl_common.h"
#include "systime.h"

#define SEND_BYTES_ONCE 17
#define SEND_TIMEOUT (SYSTIME_SECOND(2))

void SgpBleAgent_Init(void);
void SgpBleAgent_Process(void);

void SgpBleAgent_ChangeConnStatus(bool isConnected);
void SgpBleAgent_Receive(const uint8_t* data, uint8_t length);

