#include "ws2812Matrix.h"

/*
 * PWM刷新一帧8*32点WS2812B矩阵时间为1.25us*(80+8*32*3*8)=7780us=7.78ms
 * */

#define RESET_ZERO_NUM  80
#define WS2812_ZERO     29
#define WS2812_ONE      59

uint8_t pwmFrame[RESET_ZERO_NUM+MATRIX_ROW*MATRIX_COLUMN*3*8] = {0};	/* PWM控制器DMA帧RAM */
uint8_t (*pwmMatrix)[MATRIX_COLUMN][MATRIX_ROW][3][8] = (uint8_t (*)[MATRIX_COLUMN][MATRIX_ROW][3][8])(pwmFrame+RESET_ZERO_NUM);
rgbPoint_u matrixRam[MATRIX_ROW][MATRIX_COLUMN] = {0};	                /* LED点阵映射 */
struct rt_semaphore pwmSem;     /* LED点阵PWM刷新锁 */
struct rt_mutex screenMutex;    /* LED屏幕互斥锁 */

extern uint8_t getBrightness(void);

/**
 * @brief 获取PWM DMA缓存地址区域
 * 
 * @return void* PWM DMA数据区域指针
 */
void* getPwmDmaRamAddr(void)
{
    return (void*)pwmFrame;
}

/**
 * @brief 获取PWM DMA发送区域大小
 * 
 * @return int PWM DMA发送大小
 */
int getPwmDmaRamSize(void)
{
    return sizeof(pwmFrame);
}

/**
 * @brief 图像矩阵转换为PWM数据
 * 
 */
void matrix2pwm(void)
{
	int row, column, color, bit;
    rgbPoint_u pixel;
    uint8_t brightness = getBrightness();

	for(row=0; row<MATRIX_ROW; row++)
		for(column=0; column<MATRIX_COLUMN; column++)
        {
            pixel.color = matrixRam[row][column].color;
            adjustPixelBrightness(brightness, &pixel, 1);
		    for(color=0; color<3; color++)
		        for(bit=0; bit<8; bit++)
		            if(pixel.color & (0x800000>>(color*8+bit)))
		                (*pwmMatrix)[column][column%2 ? (MATRIX_ROW-1-row) : row][color][bit] = WS2812_ONE;
		            else
		                (*pwmMatrix)[column][column%2 ? (MATRIX_ROW-1-row) : row][color][bit] = WS2812_ZERO;
        }
}

/**
 * @brief 显示文字字模
 * 
 * @param x x坐标
 * @param y y坐标
 * @param font 字模
 * @param fontColor 文字颜色
 */
void displayChar(int x, int y, const font_t *font, const rgbPoint_u fontColor)
{
    int point = 0, bit = 0;
    int row = y, column = x;

    while(point < font->width*font->hight)
    {
        for(bit=0; bit<8; bit++)
        {
            if(font->pixel[point/8] & (0x80>>bit))
                matrixRam[row][column].color = fontColor.color;
            else
                matrixRam[row][column].color = 0x0;

            column++;
            if(column == x+font->width)
            {
                row++;
                column = x;
            }
            point++;
        }
    }
}

/**
 * @brief 显示图案
 * 
 * @param x x坐标
 * @param y y坐标
 * @param pattern 图案
 */
void displayPattern(int x, int y, const pattern_t *pattern)
{
    int point = 0;
    int row = y, column = x;

    while(point < pattern->width*pattern->hight)
    {
        matrixRam[row][column].color = pattern->pixel[point].color;

        column++;
        if(column == x+pattern->width)
        {
            row++;
            column = x;
        }
        point++;
    }
}

/**
 * @brief 屏幕刷新（需要先获取互斥量）
 * 
 * @return int 0:开始刷新
 */
int screenRefresh(void)
{
    rt_sem_take(&pwmSem, RT_WAITING_FOREVER);
    matrix2pwm();
    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)getPwmDmaRamAddr(), getPwmDmaRamSize());
    return 0;
}

/**
 * @brief 点阵屏初始化
 * 由于使用了调用了screenRefresh，需要在main函数中初始化，不能导出到rtt初始化代码段
 * 
 * @return int 
 */
int matrixScreenInit(void)
{
    rt_sem_init(&pwmSem, "pwm_sem", 1, RT_IPC_FLAG_FIFO);
    rt_mutex_init(&screenMutex, "scrn", RT_IPC_FLAG_FIFO);

    for(int i=0; i<3; i++)
    {
        screenRefresh(); /* 清空屏幕 */
    }
    return 0;
}

/**
 * @brief 获取屏幕控制互斥量
 * 
 * @return rt_err_t RT_EOK 成功获得互斥量；-RT_ETIMEOUT 超时；-RT_ERROR 获取失败
 */
rt_err_t takeScreenMutex(void)
{
    return rt_mutex_take(&screenMutex, RT_WAITING_FOREVER);
}

/**
 * @brief 释放屏幕控制互斥量
 * 
 * @return rt_err_t RT_EOK 成功
 */
rt_err_t releaseScreenMutex(void)
{
    return rt_mutex_release(&screenMutex);
}

/**
 * @brief PWM传输完成中断回调函数
 * 
 * @param htim TIM设备控制句柄
 */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    rt_interrupt_enter();
    HAL_TIM_PWM_Stop_DMA(&htim1,TIM_CHANNEL_1);
    rt_sem_release(&pwmSem);
    rt_interrupt_leave();
}

