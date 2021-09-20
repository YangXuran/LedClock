#ifndef __AT_H
#define __AT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include <rtthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ringbuffer.h"
#include "at_hw.h"

typedef enum {
    AT_DEV_UNKNOWN = 0,
    AT_DEV_POWER_ON,
    AT_DEVICE_WAIT_NET_CONFIG,
    AT_DEV_CONNECT_NET,
    AT_DEV_DISABLE
}AT_DEVICE_STATUS;

typedef enum {
    STATION = 1,
    SOFTAP,
    SOFTAP_STATION
}WIFI_MODE;

typedef struct{
    struct rt_mutex mutex;
    int status;
    int isDebugMode;
}atDevice_t;

#define AT_ERROR(fmt, args...) \
            do{if(1) rt_kprintf("<AT_ERR>(%s:%d): "fmt, \
                    __FUNCTION__, __LINE__, ##args); }while(0)

#define AT_DEBUG(fmt, args...) \
            do{if(espDevice.isDebugMode) {rt_kprintf("<AT_DEBUG>(%s:%d): "fmt, \
                    __FUNCTION__, __LINE__, ##args);} }while(0)

#define AT_SEND(cmd) \
            do{if(espDevice.isDebugMode) {rt_kprintf("<AT_SEND>(%s:%d): ##", \
                    __FUNCTION__, __LINE__); \
                    printAtCmd(cmd);	\
                    rt_kprintf("##\n");} }while(0)

#define AT_RECV(cmd) \
            do{if(espDevice.isDebugMode) {rt_kprintf("<AT_RECV>(%s:%d): ##", \
                    __FUNCTION__, __LINE__); \
                    printAtCmd(cmd);	\
                    rt_kprintf("##\n");} }while(0)

int usrAtTask(int arg);
#ifdef __cplusplus
}
#endif

#endif /* __AT_H */
