/*
 * task_handler.c
 *
 *  Created on: Sep 10, 2025
 *      Author: argyris
 */

#include "task_handler.h"

void menu_task(void *param){
	while(1){

	}
}

void led_task(void *param){
	while(1){
		HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
		HAL_Delay(100);
	}
}

void rtc_task(void *param){
	while(1){

	}
}

void print_task(void *param){
	while(1){

	}
}

void cmd_handler_task(void *param){
	while(1){

	}
}
