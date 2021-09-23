#include "at_hw.h"

#define AT_DEV_UART USART3

uartDevice_t atUartDev;

/************************* CHIP UART *******************************/

/**
 * @brief AT指令串口初始化
 * 
 * @return int 0:成功，-1:失败
 */
int atUartInit(void)
{
    atUartDev.handler.Instance = AT_DEV_UART;
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

    return 0;
}
INIT_BOARD_EXPORT(atUartInit);

/**
 * @brief 发送AT指令
 * 
 * @param data 指令指针
 * @param length 指令长度
 * @return int 0:成功，-1:参数错误
 */
int sendAtData(char *data, int length)
{
    int putLen;

    if(data == NULL)
        return -1;

    do{
        putLen = rt_ringbuffer_put(&atUartDev.txRingCb, (uint8_t*)data, length);
        __HAL_UART_ENABLE_IT(&atUartDev.handler, UART_IT_TXE);
        length -= putLen;
        data += putLen;
        rt_thread_mdelay(1);
    }while(length != 0);

    return 0;
}

/**
 * @brief 接收AT指令应答
 * 
 * @param buffer 接收缓冲区指针
 * @param length 接受缓冲区长度
 * @param timeout 超时等待时间，单位ms
 * @return int 实际接收数据长度
 */
int receiveAtData(char *buffer, int length, int timeout)
{
    int getLen = 0, receiveCount = 0;
    rt_err_t ret;

    getLen = rt_ringbuffer_get(&atUartDev.rxRingCb, (uint8_t*)buffer, length);
    receiveCount += getLen;
    while(receiveCount != length)
    {
    	rt_thread_mdelay(1);
//        ret = rt_sem_take(&atUartDev.rxSem, timeout);
//        if(ret == -RT_ETIMEOUT)
//            return receiveCount;
//        else
//            getLen = rt_ringbuffer_get(&atUartDev.rxRingCb, (uint8_t*)(buffer+receiveCount), length-receiveCount);
    	getLen = rt_ringbuffer_get(&atUartDev.rxRingCb, (uint8_t*)(buffer+receiveCount), length-receiveCount);
    	if(getLen == 0 && timeout <= 0)
    		break;
    	timeout--;
    	receiveCount += getLen;
    }
    return receiveCount;
}

/**
 * @brief 清除接收缓冲区数据
 * 
 * @return int 返回0
 */
int clearReceiveRngBuff(void)
{
    rt_ringbuffer_reset(&atUartDev.rxRingCb);
    return 0;
}

/**
 * @brief UART3 IRQ Handler
 * 
 */
void USART3_IRQHandler(void)
{
    UART_HandleTypeDef *huart = &atUartDev.handler;
    uint32_t isrflags   = READ_REG(huart->Instance->SR);
    uint32_t cr1its     = READ_REG(huart->Instance->CR1);
    uint32_t cr3its     = READ_REG(huart->Instance->CR3);
    uint32_t errorflags = 0x00U;
    uint8_t dataByte;
    uint32_t length;
    uint32_t nouse;

    UNUSED(cr3its);
    UNUSED(nouse);
    rt_interrupt_enter();
    errorflags = (isrflags & (uint32_t)(USART_SR_PE | USART_SR_FE | USART_SR_ORE | USART_SR_NE));
    if (errorflags == RESET)
    {
      /* UART in mode Receiver -------------------------------------------------*/
      if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
      {
          dataByte = (uint8_t)(huart->Instance->DR & (uint8_t)0x00FF);
          rt_ringbuffer_put(&atUartDev.rxRingCb, &dataByte, 1);
//          rt_sem_release(&atUartDev.rxSem);
      }
      /* UART in mode Transmitter ------------------------------------------------*/
      if (((isrflags & USART_SR_TXE) != RESET) && ((cr1its & USART_CR1_TXEIE) != RESET))
      {
          length = rt_ringbuffer_get(&atUartDev.txRingCb, &dataByte, 1);
          if(length == 0)
              __HAL_UART_DISABLE_IT(&atUartDev.handler, UART_IT_TXE);
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

/************************* CHIP PIN *******************************/

/**
 * @brief 使能ESP8266芯片
 * 
 * @return int 返回0
 */
int esp8266CtrlGpioInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); /* 使能脚 */

    GPIO_InitStruct.Pin = GPIO_PIN_9;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); /* 复位脚 */

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 1); /*芯片使能*/
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, 1); /* 释放复位 */

    return 0;
}
INIT_BOARD_EXPORT(esp8266CtrlGpioInit);

/**
 * @brief ESP8266硬复位
 * 
 */
void esp8266HardReset(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, 0);
    rt_thread_mdelay(100);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, 1);
}


