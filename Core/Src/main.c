/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <rtthread.h>
#include "rtdbg.h"
#include "ws2812Matrix.h"
#include "fonts.h"
#include "at.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TIME_X              10
#define TIME_Y              1
#define FRAME_PRE_SECOND    60
#define DEFAULE_TIME_COLOR  0x303030
#define MAGIC_TIME_COLOR    0xFF0000

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* 任务栈与控制块 */
ALIGN(RT_ALIGN_SIZE)
static char clockDisplayTask_stack[1024];
static struct rt_thread clockDisplayTask_tb;

ALIGN(RT_ALIGN_SIZE)
static char time_stack[4096];
static struct rt_thread time_tb;

ALIGN(RT_ALIGN_SIZE)
static char wifiCtrl_stack[1024];
static struct rt_thread wifiCtrl_tb;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t getBrightness(void)
{
    return 255;
}

void clockDisplayTask(int arg)
{
    int frameCount, clkNum;
    uint32_t rtcTime_u32 = 0x888888, rtcTimeLast_u32 = 0x888888;
    uint8_t digit, digitLast;
    RTC_TimeTypeDef rtcTime;
    font_t font, fontLast;
    rgbPoint_u timeColor, timeColorLast;
    rgbPoint_u mixedPatternPixel[TIME_NUMBER_WIDTH*TIME_NUMBER_HIGHT];
    pattern_t mixedPattern;

    rt_thread_mdelay(1000);
    takeScreenMutex();
    timeColor.color = DEFAULE_TIME_COLOR;
    getTimeFonts(10, &font);
    displayChar(TIME_X+7, TIME_Y, &font, timeColor);
    for(clkNum=0; clkNum<4; clkNum++)
    {
        /* 时间取模 */
        digit =(rtcTime_u32>>(20-4*clkNum))&0xF;
        digitLast = (rtcTimeLast_u32>>(20-4*clkNum))&0xF;
        getTimeFonts(digit, &font);
        timeColor.color = DEFAULE_TIME_COLOR;
        displayChar(clkNum>1 ? TIME_X+2+clkNum*(font.width+1) : TIME_X+clkNum*(font.width+1), TIME_Y, &font, timeColor);
    }
    releaseScreenMutex();

    while(1)
    {
        HAL_RTC_GetTime(&hrtc, &rtcTime, RTC_FORMAT_BCD);
        /* TODO: 因为不显示秒，后面需要注释，现在为了调试方便把秒加上每秒都可以触发断点 */
        rtcTime_u32 = rtcTime.Hours<<16 | rtcTime.Minutes<<8 | rtcTime.Seconds;
        if(rtcTime_u32 == rtcTimeLast_u32)
        {
            /* nothing to do here */
        }else
        {
            takeScreenMutex();
            for(frameCount=0; frameCount<FRAME_PRE_SECOND; frameCount++)
            {
                /* 时与分间冒号 */
                timeColor.color = DEFAULE_TIME_COLOR;
                getTimeFonts(10, &font);
                displayChar(TIME_X+7, TIME_Y, &font, timeColor);
                for(clkNum=0; clkNum<4; clkNum++)
                {
                    /* 时间取模 */
                    digit =(rtcTime_u32>>(20-4*clkNum))&0xF;
                    digitLast = (rtcTimeLast_u32>>(20-4*clkNum))&0xF;
                    if(digit != digitLast)
                    {
                        getTimeFonts(digit, &font);
                        getTimeFonts(digitLast, &fontLast);
                        timeColor.color = DEFAULE_TIME_COLOR;
                        timeColorLast.color = DEFAULE_TIME_COLOR;
                        if(frameCount < FRAME_PRE_SECOND-1)
                        {
                            mixedPattern.hight = font.hight;
                            mixedPattern.width = font.width;
                            mixedPattern.pixel = mixedPatternPixel;
                            adjustPixelBrightness(frameCount*(1.0*255/FRAME_PRE_SECOND), &timeColor, 1);
                            adjustPixelBrightness(255-(frameCount*(1.0*255/FRAME_PRE_SECOND)), &timeColorLast, 1);
                            mixChar2Pattern(&mixedPattern,
                                            &font, timeColor,
                                            &fontLast, timeColorLast);
                            displayPattern(clkNum>1 ? TIME_X+2+clkNum*(font.width+1) : TIME_X+clkNum*(font.width+1), TIME_Y, &mixedPattern);
                        }else
                        {
                            displayChar(clkNum>1 ? TIME_X+2+clkNum*(font.width+1) : TIME_X+clkNum*(font.width+1), TIME_Y, &font, timeColor);
                        }
                    }
                }

                screenRefresh();
                rt_thread_mdelay((1000/FRAME_PRE_SECOND)-100);
            }
            releaseScreenMutex();
            rtcTimeLast_u32 = rtcTime_u32;
        }
        rt_thread_mdelay(100);
    }
}

void showWeather(void)
{
  int index = 0;
  pattern_t wifiPattern;

  rt_thread_mdelay(1100);
  while(getWifiStatus() != AT_DEV_CONNECT_NET)
  {
    takeScreenMutex();
    getWifiPattern(index, &wifiPattern);
    displayPattern(1, 0, &wifiPattern);
    screenRefresh();
    releaseScreenMutex();
    rt_thread_mdelay(1000);
    index = index ? 0 : 1;
  }
 
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
    RTC_TimeTypeDef rtcTime;
    RTC_DateTypeDef rtcDate;
 goto threadInit; /* Avoid automatic generation of initialization code*/
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_RTC_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
threadInit:
  HAL_RTC_GetDate(&hrtc, &rtcDate, RTC_FORMAT_BCD);
  HAL_RTC_GetTime(&hrtc, &rtcTime, RTC_FORMAT_BCD);
  rt_kprintf("System Start At: 20%02x/%02x/%02x  ", rtcDate.Year, rtcDate.Month, rtcDate.Date);
  rt_kprintf("%02x:%02x:%02x\n",rtcTime.Hours, rtcTime.Minutes, rtcTime.Seconds);

  matrixScreenInit(); /* 点阵屏初始化 */

  /* 时间显示线程 */
  rt_thread_init(&clockDisplayTask_tb,
                  "clk_dpy",
                  (void(*))&clockDisplayTask,
                  NULL,
                  clockDisplayTask_stack,
                  sizeof(clockDisplayTask_stack),
                  10,
                  5);
  rt_thread_startup(&clockDisplayTask_tb);

  /* 时间校准线程 */
  rt_thread_init(&time_tb,
                  "time_calibration",
                  (void(*))&timeCalibration,
                  NULL,
                  time_stack,
                  sizeof(time_stack),
                  12,
                  5);
  rt_thread_startup(&time_tb);

  /* WIFI模块控制线程 */
  rt_thread_init(&wifiCtrl_tb,
                  "wifi_ctrl",
                  (void(*))&usrAtTask,
                  NULL,
                  wifiCtrl_stack,
                  sizeof(wifiCtrl_stack),
                  30,
                  5);
  rt_thread_startup(&wifiCtrl_tb);

showWeather();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM2 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM2) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
