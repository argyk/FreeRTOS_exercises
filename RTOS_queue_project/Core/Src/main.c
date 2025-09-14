/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "rtc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "FreeRTOS.h"
#include "SEGGER_SYSVIEW.h"
#include "task.h"
#include "queue.h"
#include "task_handler.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

TaskHandle_t handle_led_task;
TaskHandle_t handle_rtc_task;
TaskHandle_t handle_menu_task;
TaskHandle_t handle_print_task;
TaskHandle_t handle_cmd_task;

QueueHandle_t q_print;
QueueHandle_t q_data;

uint8_t userData;

state_t currState = sMainMenu;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    // Handle error: a task has run out of stack space
    // You could log, blink an LED, reset the MCU, etc.

    (void)xTask;
    (void)pcTaskName;

    // For debugging: stay here
    taskDISABLE_INTERRUPTS();
    for(;;);
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  BaseType_t status;
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */

  //NEED TO DEFINE THIS AGAIN because FREERTOS initializes this in vTaskStartScheduler, but SEGGER_SYSVIEW_start() needs it earlier!!!
  //cannot start without it (it is initialized by HAL after the scheduler start)
  NVIC_SetPriorityGrouping(0);  // same as NVIC_PRIORITYGROUP_4
  SEGGER_SYSVIEW_Conf();
  SEGGER_SYSVIEW_Start();

  status = xTaskCreate(menu_task, "Menu Task", 128, NULL, 2, &handle_menu_task);
  configASSERT(status == pdPASS);

  status = xTaskCreate(led_task, "LED Task", 512, NULL, 1, &handle_led_task);
  configASSERT(status == pdPASS);

  status = xTaskCreate(rtc_task, "RTC Task", 128, NULL, 1, &handle_rtc_task);
  configASSERT(status == pdPASS);

  status = xTaskCreate(print_task, "Print Task", 128, NULL, 1, &handle_print_task);
  configASSERT(status == pdPASS);

  status = xTaskCreate(cmd_handler_task, "Command Handler Task", 128, NULL, 1, &handle_cmd_task);
  configASSERT(status == pdPASS);

  q_print = xQueueCreate(10 , sizeof(size_t));
  configASSERT(q_print != NULL);

  q_data = xQueueCreate(10 , sizeof(char));
  configASSERT(q_data != NULL);

  HAL_UART_Receive_IT(&huart2, &userData, 1);

  vTaskStartScheduler();

//  char data[] = "Hello World !\r\n";

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  }
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
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
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

//This executes in interrupt context when receive from UART is completed.
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	BaseType_t status;
	uint8_t temp;

	if (! xQueueIsQueueFullFromISR(q_data)){
		status = xQueueSendToBackFromISR(q_data, &userData, NULL);
		configASSERT(status == pdPASS);
	}
	else {
		if (userData == '\n'){
			//need to force the last character to \n
			xQueueReceiveFromISR(q_data, (void*)&temp, NULL);
			xQueueSendToBackFromISR(q_data, &userData, NULL);
		}
	}
	if (userData == '\n'){
		//Notify cmd task
		xTaskNotifyFromISR(handle_cmd_task, 0, eNoAction, NULL);
	}
	//echo char (non blocking transmit through Interrupt mode
	HAL_UART_Transmit_IT(&huart2, (uint8_t*) &userData, strlen((char*)&userData));
	//Receive next character
	HAL_UART_Receive_IT(&huart2, &userData, 1);


}

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6)
  {
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
#ifdef USE_FULL_ASSERT
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
