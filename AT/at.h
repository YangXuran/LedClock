#ifndef __AT_H
#define __AT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include <rtthread.h>
#include "ringbuffer.h"
#include <string.h>
#include "at_hw.h"

typedef struct{
    struct rt_semaphore mutex;
    int isDebugMode;
    int status;
}atDevice_t;

#ifdef __cplusplus
}
#endif

#endif /* __AT_H */
