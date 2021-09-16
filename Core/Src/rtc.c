/**
  ******************************************************************************
  * @file    rtc.c
  * @brief   This file provides code for the configuration
  *          of the RTC instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rtc.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

/* RTC init function */
void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef DateToUpdate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
  /* TODO: STM32F1无法保存日期，需要手动写入备份寄存器中 */
  if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) ==  0x5A5A)
      return;

  HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x5A5A);
  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  DateToUpdate.WeekDay = RTC_WEEKDAY_MONDAY;
  DateToUpdate.Month = RTC_MONTH_JANUARY;
  DateToUpdate.Date = 0x1;
  DateToUpdate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */
    HAL_PWR_EnableBkUpAccess();
    /* Enable BKP CLK enable for backup registers */
    __HAL_RCC_BKP_CLK_ENABLE();
    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
int sysRtcInit(void)
{
    MX_RTC_Init();
    return 0;
}
INIT_BOARD_EXPORT(sysRtcInit);

int printRtcTime(void)
{
    RTC_TimeTypeDef rtcTime;
    RTC_DateTypeDef rtcDate;

    HAL_RTC_GetDate(&hrtc, &rtcDate, RTC_FORMAT_BCD);
    HAL_RTC_GetTime(&hrtc, &rtcTime, RTC_FORMAT_BCD);
    rt_kprintf("20%02x/%02x/%02x  ", rtcDate.Year, rtcDate.Month, rtcDate.Date);
    rt_kprintf("%02x:%02x:%02x\n",rtcTime.Hours, rtcTime.Minutes, rtcTime.Seconds);
    return 0;
}
MSH_CMD_EXPORT(printRtcTime, "Print RTC date and time");

int calculateWeek(int year, int month, int day)
{
    int c,y,week;
    if (month == 1 || month == 2)
    year--, month += 12;
    c = year / 100;
    y = year - c * 100;
    week = y + y / 4 + c / 4 - 2 * c + 26 * (month + 1) / 10 + day - 1;
    while (week < 0)
    week += 7;
    week %= 7;
    return week;
}

int setDate(int argc, char *argv[])
{
    int i, j, strLen;
    RTC_DateTypeDef rtcDate = {0};
    char *date = argv[1];
    uint32_t formatDate[4] = {0};

    if(argc < 2)
        goto err;

    strLen = strlen(date);
    for(i=j=0; i<strLen; i++)
        if((date[i]>='0') && (date[i]<='9'))
            formatDate[j] = (formatDate[j]<<4) | (date[i]-'0');
        else
            j++;

    if(formatDate[0] > 0x2099 || formatDate[1] > 0x12 || formatDate[2] > 0x31)
        goto err;

    formatDate[3] = calculateWeek(((formatDate[0]>>12)&0xF)*1000+((formatDate[0]>>8)&0xF)*100+((formatDate[0]>>4)&0xF)*10+(formatDate[0]&0xF),
                                  ((formatDate[1]>>4)&0xF)*10+(formatDate[1]&0xF),
                                  ((formatDate[2]>>4)&0xF)*10+(formatDate[2]&0xF));
    if(formatDate[3] == 0)
        formatDate[3] = 7;
    rtcDate.Year = formatDate[0] & 0xFF;
    rtcDate.Month = formatDate[1] & 0xFF;
    rtcDate.Date = formatDate[2] & 0xFF;
    rtcDate.WeekDay = formatDate[3] & 0xFF;
    rt_kprintf("Set Date:20%02x/%02x/%02x, WeekDay:%x\n", rtcDate.Year, rtcDate.Month, rtcDate.Date, rtcDate.WeekDay);
    if (HAL_RTC_SetDate(&hrtc, &rtcDate, RTC_FORMAT_BCD) != HAL_OK)
    {
        rt_kprintf("Set Date Error\n");
        return -1;
    }
    return 0;
err:
    rt_kprintf("Parameter error, e.g., setDate 2021/09/11\n");
    return -1;
}
MSH_CMD_EXPORT(setDate, "Set RTC date, e.g. setDate 2021/09/11");

int setTime(int argc, char *argv[])
{
    int i, j, strLen;
    RTC_TimeTypeDef rtcTime = {0};
    char *date = argv[1];
    uint32_t formatDate[2] = {0};

    if(argc < 2)
        goto err;

    strLen = strlen(date);
    for(i=j=0; i<strLen; i++)
        if((date[i]>='0') && (date[i]<='9'))
            formatDate[j] = (formatDate[j]<<4) | (date[i]-'0');
        else
            j++;

    if(formatDate[0] > 0x23 || formatDate[1] > 0x59)
        goto err;

    rtcTime.Hours = formatDate[0];
    rtcTime.Minutes = formatDate[1];
    rtcTime.Seconds = 0x0;
    rt_kprintf("Set Time: %02x:%02x\n", rtcTime.Hours, rtcTime.Minutes);
    if (HAL_RTC_SetTime(&hrtc, &rtcTime, RTC_FORMAT_BCD) != HAL_OK)
    {
        rt_kprintf("Set Date Error\n");
        return -1;
    }
    return 0;
err:
    rt_kprintf("Parameter error, e.g., setTime 21:39\n");
    return -1;
}
MSH_CMD_EXPORT(setTime, "Set RTC time, e.g. setTime 21:39");
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
