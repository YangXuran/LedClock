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
#include "easyflash.h"
#include "cJSON.h"
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
#define DEFAULE_TIME_COLOR  0x808080
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

ALIGN(RT_ALIGN_SIZE)
static char weather_stack[8192];
static struct rt_thread weather_tb;

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
    RTC_DateTypeDef rtcDate;
    font_t font, fontLast;
    rgbPoint_u timeColor, timeColorLast;
    rgbPoint_u mixedPatternPixel[TIME_NUMBER_WIDTH*TIME_NUMBER_HIGHT];
    pattern_t mixedPattern;
    pattern_t weekDayPattern;

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
        HAL_RTC_GetDate(&hrtc, &rtcDate, RTC_FORMAT_BCD);
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
                generateWeekDayPattern(rtcDate.WeekDay ? rtcDate.WeekDay : 7, &weekDayPattern);
                displayPattern(28, 0, &weekDayPattern);
                rt_free(weekDayPattern.pixel);
                screenRefresh();
                rt_thread_mdelay((1000/FRAME_PRE_SECOND)-100);
            }
            releaseScreenMutex();
            rtcTimeLast_u32 = rtcTime_u32;
        }
        rt_thread_mdelay(100);
    }
}

void showWeather(int arg)
{
  int ret = 0, index = 0;
  pattern_t wifiPattern;
  pattern_t weatherIcon;
  pattern_t tempPattern;
  char url[256] = {0};
  char key[64] = {0};
  char location[32] = {0};
  char weatherJson[1024] = {0};
  cJSON *root = NULL;
  cJSON *nowJSON = NULL;
  cJSON *iconJSON = NULL;
  cJSON *tempJSON = NULL;

  rt_thread_mdelay(1100);
  while(1)
  {
    while(getWifiStatus() != AT_DEV_CONNECT_NET)
    {
      takeScreenMutex();
      getWifiPattern(index, &wifiPattern);
      generateTemperaturePattern(-273, &tempPattern);
      displayPattern(1, 0, &wifiPattern);
      displayPattern(9, 7, &tempPattern);
      screenRefresh();
      releaseScreenMutex();
      rt_free(tempPattern.pixel);
      rt_thread_mdelay(1000);
      index = index ? 0 : 1;
    }
    ret = ef_get_env_blob("wearher_key", key, sizeof(key), NULL);
    if(ret == 0)
    {
      rt_thread_mdelay(5000);
      continue;
    }

    ret = ef_get_env_blob("wearher_loc", location, sizeof(location), NULL);
    if(ret == 0)
    {
      rt_thread_mdelay(5000);
      continue;
    }
    snprintf(url, sizeof(url), "https://devapi.qweather.com/v7/weather/now?key=%s&location=%s&gzip=n", key, location);
    rt_thread_mdelay(1000);
    ret = simpleHttpGet(url, weatherJson, sizeof(weatherJson), 2000);
    if(ret <= 0)
      continue;
    root = cJSON_Parse(weatherJson);
    if(root == NULL){
      rt_kprintf("cJSON_Parse fail![%s]\n",cJSON_GetErrorPtr());
      continue;
    }
    nowJSON = cJSON_GetObjectItem(root, "now");
    if(nowJSON == NULL)
    {
        cJSON_Delete(root);
        continue;
    }
    char *weatherData = cJSON_Print(nowJSON);
    rt_kprintf("WeatherData天气数据:\n%s\n\n", weatherData);
    cJSON_free(weatherData);
    iconJSON = cJSON_GetObjectItem(nowJSON, "icon");
    tempJSON = cJSON_GetObjectItem(nowJSON, "temp");
    if (iconJSON != NULL)
    {
      char *icon = cJSON_Print(iconJSON);
      char *temp = cJSON_Print(tempJSON);
      rt_kprintf("\n\nget weather icon:%d, temperature:%d\n", atoi(icon+1), atoi(temp+1));
      getWeatherPattern(atoi(icon+1), &weatherIcon);
      generateTemperaturePattern(atoi(temp+1), &tempPattern);
      cJSON_free(icon);
      cJSON_free(temp);
      takeScreenMutex();
      displayPattern(1, 0, &weatherIcon);
      displayPattern(9, 7, &tempPattern);
      screenRefresh();
      releaseScreenMutex();
      rt_free(tempPattern.pixel);
    }
    cJSON_Delete(root);
    rt_thread_mdelay(20*60*1000);
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
    cJSON_Hooks cJSONHooks = {.malloc_fn = (void * (*)(unsigned int))&rt_malloc,
                              .free_fn = &rt_free};
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
  easyflash_init();
  cJSON_InitHooks(&cJSONHooks);
  HAL_RTC_GetDate(&hrtc, &rtcDate, RTC_FORMAT_BCD);
  HAL_RTC_GetTime(&hrtc, &rtcTime, RTC_FORMAT_BCD);
  rt_kprintf("System Start At: 20%02x/%02x/%02x  ", rtcDate.Year, rtcDate.Month, rtcDate.Date);
  rt_kprintf("%02x:%02x:%02x\n",rtcTime.Hours, rtcTime.Minutes, rtcTime.Seconds);

  matrixScreenInit(); /* 点阵屏初始化 */

  rt_thread_mdelay(1100);

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

  /* 天气显示控制线程 */
  rt_thread_init(&weather_tb,
                  "weather",
                  (void(*))&showWeather,
                  NULL,
                  weather_stack,
                  sizeof(weather_stack),
                  29,
                  5);
  rt_thread_startup(&weather_tb);


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

int showWeatherIcon(int argc, char *argv[])
{
    pattern_t weatherIcon;

    getWeatherPattern(atoi(argv[1]), &weatherIcon);
    takeScreenMutex();
    displayPattern(1, 0, &weatherIcon);
    screenRefresh();
    releaseScreenMutex();
    return 0;
}
MSH_CMD_EXPORT(showWeatherIcon, Show weather icon);

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
