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
#include <stdbool.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FreeRTOS.h"
#include "task.h"

#include "SEGGER_SYSVIEW.h"
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
TaskHandle_t xGreenLEDTaskHandle;
TaskHandle_t xRedLEDTaskHandle;
//TaskHandle_t xBlueLEDTaskHandle;
//TaskHandle_t xExtButtonTaskHandle;
bool button_status = 0;
//TaskHandle_t volatile next_task_handle = NULL;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void vGreenLEDTask(void *pvParameters);
void vRedLEDTask(void *pvParameters);
//void vBlueLEDTask(void *pvParameters);
//void vExtButtonTask(void *pvParameters);

void button_interrupt_handler(void);
void check_button(void);


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

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  BaseType_t status;

  //NEED TO DEFINE THIS AGAIN because FREERTOS initializes this in vTaskStartScheduler, but SEGGER_SYSVIEW_start() needs it earlier!!!
  //cannot start without it (it is initialized by HAL after the scheduler start)
  NVIC_SetPriorityGrouping(0);  // same as NVIC_PRIORITYGROUP_4
  SEGGER_SYSVIEW_Conf();
  SEGGER_SYSVIEW_Start();

//  SEGGER_SYSVIEW_Print("Hello SystemView!");

  status = xTaskCreate(vGreenLEDTask, "GREEN_LED", 200, NULL,3, &xGreenLEDTaskHandle);
  configASSERT(status == pdPASS);

  status = xTaskCreate(vRedLEDTask, "RED_LED", 200, NULL,2, &xRedLEDTaskHandle);
  configASSERT(status == pdPASS);

//  status = xTaskCreate(vBlueLEDTask, "BLUE_LED", 200, NULL,2, &xBlueLEDTaskHandle);
//  configASSERT(status == pdPASS);

//  next_task_handle = xGreenLEDTaskHandle;

//  status = xTaskCreate(vExtButtonTask, "EXT_BUTTON", 200, NULL,4, &xExtButtonTaskHandle);
//  configASSERT(status == pdPASS);

  vTaskStartScheduler();
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
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
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED_RED_Pin|LED_BLUE_Pin|LED_GREEN_EXT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LED_RED_Pin LED_BLUE_Pin LED_GREEN_EXT_Pin */
  GPIO_InitStruct.Pin = LED_RED_Pin|LED_BLUE_Pin|LED_GREEN_EXT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : BUTTON_EXT_Pin */
  GPIO_InitStruct.Pin = BUTTON_EXT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BUTTON_EXT_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == BUTTON_EXT_Pin)
    {
        button_interrupt_handler();
    }
}

void button_interrupt_handler(void){

	//This will implement context switch to any task that exited suspended/blocked state and went to ready state
//	BaseType_t pxHigherPriorityTaskWoken = pdFALSE;

	traceISR_ENTER();

	button_status = 1;

	traceISR_EXIT();
}

void check_button(void){
	taskENTER_CRITICAL();
	if (button_status){
		UBaseType_t p1 = uxTaskPriorityGet(xGreenLEDTaskHandle);
		UBaseType_t p2 = uxTaskPriorityGet(xRedLEDTaskHandle);
		vTaskPrioritySet(xRedLEDTaskHandle, p1);
		vTaskPrioritySet(xGreenLEDTaskHandle, p2);
		button_status=0;
	}
	taskEXIT_CRITICAL();
}

void vGreenLEDTask(void *pvParameters){
	while(1){
		SEGGER_SYSVIEW_PrintfTarget("Toggling Green LED");
		HAL_GPIO_TogglePin(LED_GREEN_EXT_GPIO_Port, LED_GREEN_EXT_Pin);
		HAL_Delay(pdMS_TO_TICKS(1000));
		check_button();
	}
}

void vRedLEDTask(void *pvParameters){
	while(1){
		SEGGER_SYSVIEW_PrintfTarget("Toggling Red LED");
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		HAL_Delay(pdMS_TO_TICKS(100));
		check_button();


	}
}

//void vBlueLEDTask(void *pvParameters){
//	BaseType_t status;
//
//	while(1){
//		SEGGER_SYSVIEW_PrintfTarget("Toggling Blue LED");
//		HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
//		vTaskDelay(pdMS_TO_TICKS(100));
//
//	}
//}

//void vExtButtonTask(void *pvParameters){
//
//	GPIO_PinState btn_read = 0;
//	GPIO_PinState prev_read = 1;
//
//	while(1){
//		btn_read = HAL_GPIO_ReadPin(BUTTON_EXT_GPIO_Port, BUTTON_EXT_Pin);
//		if (btn_read){
//			if (!prev_read){
//
//					UBaseType_t prio1 = uxTaskPriorityGet(xRedLEDTaskHandle);
//					UBaseType_t prio2 = uxTaskPriorityGet(xGreenLEDTaskHandle);
//
//					vTaskPrioritySet(xRedLEDTaskHandle, prio2);
//					vTaskPrioritySet(xGreenLEDTaskHandle, prio1);
//
//				}
//
//			}
//
//		prev_read = btn_read;
//		vTaskDelay(pdMS_TO_TICKS(10)); //Only consider long press of btn (longer than 10ms)
//	}
//}

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
