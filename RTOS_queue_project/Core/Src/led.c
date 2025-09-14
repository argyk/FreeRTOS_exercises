/*
 * led.c
 *
 *  Created on: Sep 14, 2025
 *      Author: argyris
 */

#include "task_handler.h"


void led_effect_stop(void){
	turn_on_all_leds();
}

void led_effect(int mode){
	turn_on_all_leds();
}

void turn_on_all_leds(void){
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_RED_SMALL_GPIO_Port, LED_RED_SMALL_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_GREEN_EXT_GPIO_Port, LED_GREEN_EXT_Pin, GPIO_PIN_SET);
}



#include "task_handler.h"
