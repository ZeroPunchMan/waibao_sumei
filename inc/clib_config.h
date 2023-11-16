#pragma once

#include "cl_common.h"
#include "stdio.h"
#include "nrf_log.h"

#ifdef __cplusplus
extern "C" {
#endif

//-------------event type------------------
typedef enum
{
    CL_Event_RecvMsg = 0,
    CL_EventMax,
} CL_Event_t;

//---------------log-------------------------
#include "stdio.h"
#define USE_LDB_LOG
#define CL_PRINTF   NRF_LOG_INFO

#ifdef __cplusplus
}
#endif
