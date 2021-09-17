#ifndef __AT_HW_H
#define __AT_HW_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include <rtthread.h>
#include "ringbuffer.h"
#include <string.h>

typedef struct{
    UART_HandleTypeDef handler;
    uint8_t uartRxRingPool[256];
    uint8_t uartTxRingPool[256];
    struct rt_ringbuffer rxRingCb;
    struct rt_ringbuffer txRingCb;
    struct rt_semaphore rxSem;
    struct rt_semaphore txSem;
}uartDevice_t;

int atUartInit(void);
int sendAtData(char *data, int length);
int receiveAtData(char *buffer, int length, int timeout);
int clearReceiveRngBuff(void);
int esp8266CtrlGpioInit(void);
void esp8266HardReset(void);

#ifdef __cplusplus
}
#endif

#endif /* __AT_HW_H */
