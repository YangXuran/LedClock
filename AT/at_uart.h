#ifndef __AT_UART_H
#define __AT_UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include <rtthread.h>

typedef struct{
    UART_HandleTypeDef handler;
    uint8_t uartRxRingPool[256];
    uint8_t uartTxRingPool[256];
    struct rt_ringbuffer rxRingCb;
    struct rt_ringbuffer txRingCb;
    struct rt_semaphore rxSem;
    struct rt_semaphore txSem;
}uartDevice_t;

#ifdef __cplusplus
}
#endif

#endif /* __AT_UART_H */
