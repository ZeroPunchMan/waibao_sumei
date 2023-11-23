#pragma once

#include "cl_common.h"

void SysOutput_Init(void);
void SysOutput_Process(void);

void SysOutput_Stop();
void SysOutput_RunPause(bool en);
void SysOutput_SetChannel(uint8_t chan, uint8_t power, uint8_t freq);

