/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtc.c
  * @brief   This file provides code for the configuration
  *          of the RTC instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
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

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_12;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
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
void show_time_date(void){

	static char display_time[50];
	static char display_date[80];
	static char *time = display_time;
	static char *date = display_date;

	RTC_TimeTypeDef rtc_time;
	RTC_DateTypeDef rtc_date;

	//Initialise to zero
	memset(&rtc_date, 0, sizeof(rtc_date));
	memset(&rtc_time, 0, sizeof(rtc_time));

	//Get current time and date
	HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);

	char* format;
	format = (rtc_time.TimeFormat == RTC_HOURFORMAT12_AM) ? "AM" : "PM";

	//Display time format: HH:MM:SS [AM/PM]
	sprintf((char*)display_time,
			"%s:\r\n\r\n\t\t%02d:%02d:%02d [%s]\r\n",
			"\r\nCurrent time & date",
			rtc_time.Hours,
			rtc_time.Minutes,
			rtc_time.Seconds,
			format);
	xQueueSendToBack(q_print, &time, portMAX_DELAY);

	char weekday[12]="";
	switch (rtc_date.WeekDay){
	case RTC_WEEKDAY_MONDAY:
		strcat(weekday, "MONDAY");
		break;
	case RTC_WEEKDAY_TUESDAY:
		strcat(weekday, "TUESDAY");
		break;
	case RTC_WEEKDAY_WEDNESDAY:
		strcat(weekday, "WEDNESDAY");
		break;
	case RTC_WEEKDAY_THURSDAY:
		strcat(weekday, "THURSDAY");
		break;
	case RTC_WEEKDAY_FRIDAY:
		strcat(weekday, "FRIDAY");
		break;
	case RTC_WEEKDAY_SATURDAY:
		strcat(weekday, "SATURDAY");
		break;
	case RTC_WEEKDAY_SUNDAY:
		strcat(weekday, "SUNDAY");
		break;
	default:
		strcat(weekday, "UNKNOWN");
		break;
	}

	//Display date format: DD-MM-YYYY
	sprintf((char*)display_date,
			"\t%s\t%02d-%02d-20%02d\n",
			weekday,
			rtc_date.Date,
			rtc_date.Month,
			rtc_date.Year);
	xQueueSendToBack(q_print, &date, portMAX_DELAY);

}

void show_time_date_itm(void){

	RTC_TimeTypeDef rtc_time;
	RTC_DateTypeDef rtc_date;

	//Initialise to zero
	memset(&rtc_date, 0, sizeof(rtc_date));
	memset(&rtc_time, 0, sizeof(rtc_time));

	//Get current time and date
	HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);

	char* format;
	format = (rtc_time.TimeFormat == RTC_HOURFORMAT12_AM) ? "AM" : "PM";

	//Display time format: HH:MM:SS [AM/PM]
	printf("\r\n%02d:%02d:%02d [%s]\r\n",
			rtc_time.Hours,
			rtc_time.Minutes,
			rtc_time.Seconds,
			format);

	char weekday[12]="";
	switch (rtc_date.WeekDay){
	case RTC_WEEKDAY_MONDAY:
		strcat(weekday, "MONDAY");
		break;
	case RTC_WEEKDAY_TUESDAY:
		strcat(weekday, "TUESDAY");
		break;
	case RTC_WEEKDAY_WEDNESDAY:
		strcat(weekday, "WEDNESDAY");
		break;
	case RTC_WEEKDAY_THURSDAY:
		strcat(weekday, "THURSDAY");
		break;
	case RTC_WEEKDAY_FRIDAY:
		strcat(weekday, "FRIDAY");
		break;
	case RTC_WEEKDAY_SATURDAY:
		strcat(weekday, "SATURDAY");
		break;
	case RTC_WEEKDAY_SUNDAY:
		strcat(weekday, "SUNDAY");
		break;
	default:
		strcat(weekday, "UNKNOWN");
		break;
	}

	//Display date format: DD-MM-YYYY
	printf("%s\t%02d-%02d-20%02d\n",
			weekday,
			rtc_date.Date,
			rtc_date.Month,
			rtc_date.Year);
}

void set_time(RTC_TimeTypeDef* time){
	HAL_RTC_SetTime(&hrtc, time, RTC_FORMAT_BIN);
}

void set_date(RTC_DateTypeDef* date){
	HAL_RTC_SetDate(&hrtc, date, RTC_FORMAT_BIN);
}


int validate_rtc_data(RTC_TimeTypeDef* time, RTC_DateTypeDef* date){
	if (time){
		if ((time->Hours > 12) || (time->Minutes > 59) || (time->Seconds >59) || time->TimeFormat >1 )
			return 1;

	}

	if (date){
		if ((date->Date >31) || (date->Month > 12) || (date->Year < 99) || (date->WeekDay>7))
			return 1;
	}

	return 0;
}

/* USER CODE END 1 */
