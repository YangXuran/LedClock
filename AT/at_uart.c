#include "at_uart.h"

static uartDevice_t atUartDev;

int atUartInit(void)
{
    atUartDev.handler.Instance = USART3;
    atUartDev.handler.Init.BaudRate = 115200;
    atUartDev.handler.Init.WordLength = UART_WORDLENGTH_8B;
    atUartDev.handler.Init.StopBits = UART_STOPBITS_1;
    atUartDev.handler.Init.Parity = UART_PARITY_NONE;
    atUartDev.handler.Init.Mode = UART_MODE_TX_RX;
    atUartDev.handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    atUartDev.handler.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&atUartDev.handler) != HAL_OK)
    {
        return -1;
    }

    rt_ringbuffer_init(&atUartDev.rxRingCb, atUartDev.uartRxRingPool, sizeof(atUartDev.uartRxRingPool));
    rt_ringbuffer_init(&atUartDev.txRingCb, atUartDev.uartTxRingPool, sizeof(atUartDev.uartTxRingPool));
    rt_sem_init(&atUartDev.rxSem, "at_uartrx_sem", 0, RT_IPC_FLAG_FIFO);
    rt_sem_init(&atUartDev.txSem, "at_uarttx_sem", 0, RT_IPC_FLAG_FIFO);

    __HAL_UART_ENABLE_IT(&atUartDev.handler, UART_IT_RXNE);
}
