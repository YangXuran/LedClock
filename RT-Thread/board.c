/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-05-24                  the first version
 */

#include <rthw.h>
#include <rtthread.h>
#include "ringbuffer.h"

#include "main.h"

typedef struct{
    uint8_t uartRxRingPool[256];
    uint8_t uartTxRingPool[256];
    struct rt_ringbuffer rxRingCb;
    struct rt_ringbuffer txRingCb;
    struct rt_semaphore rxSem;
    struct rt_semaphore txSem;
}uartDevice_t;

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
/*
 * Please modify RT_HEAP_SIZE if you enable RT_USING_HEAP
 * the RT_HEAP_SIZE max value = (sram size - ZI size), 1024 means 1024 bytes
 */
#define RT_HEAP_SIZE (10*1024)
static rt_uint8_t rt_heap[RT_HEAP_SIZE];

RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}
#endif

void SysTick_Handler(void)
{
    rt_interrupt_enter();
    
    rt_tick_increase();

    rt_interrupt_leave();
}

/**
 * This function will initial your board.
 */
void rt_hw_board_init(void)
{
    extern void SystemClock_Config(void);
    
    HAL_Init();
    SystemClock_Config();
    SystemCoreClockUpdate();
    /* 
     * 1: OS Tick Configuration
     * Enable the hardware timer and call the rt_os_tick_callback function
     * periodically with the frequency RT_TICK_PER_SECOND. 
     */
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/RT_TICK_PER_SECOND);

    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif
}

#ifdef RT_USING_CONSOLE

static UART_HandleTypeDef UartHandle;
static uartDevice_t consoleUartDev;
static int uart_init(void)
{

    UartHandle.Instance = USART1;
    UartHandle.Init.BaudRate = 115200;
    UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits = UART_STOPBITS_1;
    UartHandle.Init.Parity = UART_PARITY_NONE;
    UartHandle.Init.Mode = UART_MODE_TX_RX;
    UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&UartHandle) != HAL_OK)
    {
        while (1);
    }

    rt_ringbuffer_init(&consoleUartDev.rxRingCb, consoleUartDev.uartRxRingPool, sizeof(consoleUartDev.uartRxRingPool));
    rt_ringbuffer_init(&consoleUartDev.txRingCb, consoleUartDev.uartTxRingPool, sizeof(consoleUartDev.uartTxRingPool));
    rt_sem_init(&consoleUartDev.rxSem, "c_uartrx_sem", 0, RT_IPC_FLAG_FIFO);
    rt_sem_init(&consoleUartDev.txSem, "c_uarttx_sem", 0, RT_IPC_FLAG_FIFO);

    __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_RXNE);

    return 0;
}
INIT_BOARD_EXPORT(uart_init);

#if 0 /* RTT中如果使用中断发送，则会导致异常发生时无法打印rt_hw_hard_fault_exception中的信息 */
void rt_hw_console_output(const char *str)
{
    int i;
    rt_uint16_t size = 0;
    char a = '\r';

    __HAL_UNLOCK(&UartHandle);

    size = rt_strlen(str);

    for (i = 0; i < size; i++)
    {
        if (*(str + i) == '\n')
        {
            while(rt_ringbuffer_putchar(&consoleUartDev.txRingCb, (const rt_uint8_t)a) == 0)
                __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_TXE);
        }
        while(rt_ringbuffer_putchar(&consoleUartDev.txRingCb, (const rt_uint8_t)*(str+i)) == 0)
            __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_TXE);
    }
    __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_TXE);
}
#else
void rt_hw_console_output(const char *str)
{
    rt_size_t i = 0, size = 0;
    char a = '\r';

    __HAL_UNLOCK(&UartHandle);

    size = rt_strlen(str);
    for (i = 0; i < size; i++)
    {
        if (*(str + i) == '\n')
        {
            HAL_UART_Transmit(&UartHandle, (uint8_t *)&a, 1, 1);
        }
        HAL_UART_Transmit(&UartHandle, (uint8_t *)(str + i), 1, 1);
    }
    __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_RXNE);
}
#endif
#endif

#ifdef RT_USING_FINSH
char rt_hw_console_getchar(void)
{
    /* Note: the initial value of ch must < 0 */
    char ch;

    __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_RXNE);
    while (rt_ringbuffer_getchar(&consoleUartDev.rxRingCb, (rt_uint8_t *)&ch) != 1)
    {
        rt_sem_take(&consoleUartDev.rxSem, RT_WAITING_FOREVER);
    }
    return ch;
}
#endif


void USART1_IRQHandler(void)
{
    UART_HandleTypeDef *huart = &UartHandle;
    uint32_t isrflags   = READ_REG(huart->Instance->SR);
    uint32_t cr1its     = READ_REG(huart->Instance->CR1);
    //uint32_t cr3its     = READ_REG(huart->Instance->CR3);
    uint32_t errorflags = 0x00U;
    uint8_t dataByte;
    uint32_t length;
    volatile uint32_t nouse = 0;

    UNUSED(nouse);
    rt_interrupt_enter();
    errorflags = (isrflags & (uint32_t)(USART_SR_PE | USART_SR_FE | USART_SR_ORE | USART_SR_NE));
    if (errorflags == RESET)
    {
      /* UART in mode Receiver -------------------------------------------------*/
      if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
      {
          dataByte = (uint8_t)(huart->Instance->DR & (uint8_t)0x00FF);
          rt_ringbuffer_put(&consoleUartDev.rxRingCb, &dataByte, 1);
          rt_sem_release(&consoleUartDev.rxSem);
      }
      /* UART in mode Transmitter ------------------------------------------------*/
      if (((isrflags & USART_SR_TXE) != RESET) && ((cr1its & USART_CR1_TXEIE) != RESET))
      {
          length = rt_ringbuffer_get(&consoleUartDev.txRingCb, &dataByte, 1);
          if(length == 0)
              __HAL_UART_DISABLE_IT(&UartHandle, UART_IT_TXE);
          else
              huart->Instance->DR = (uint8_t)(dataByte & (uint8_t)0x00FF);
      }
    }else /* If some errors occur */
    {
        nouse = huart->Instance->SR;
        nouse = huart->Instance->DR;
    }
    rt_interrupt_leave();
}
