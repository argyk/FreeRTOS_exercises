/*
 * led.c
 *
 *  Created on: Sep 14, 2025
 *      Author: argyris
 */

#include "task_handler.h"



void turn_on_all_leds(void){
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_RED_SMALL_GPIO_Port, LED_RED_SMALL_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_GREEN_EXT_GPIO_Port, LED_GREEN_EXT_Pin, GPIO_PIN_SET);
}

void turn_off_all_leds(void){
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_RED_SMALL_GPIO_Port, LED_RED_SMALL_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_GREEN_EXT_GPIO_Port, LED_GREEN_EXT_Pin, GPIO_PIN_RESET);
}

void turn_on_even_leds(void){
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_GREEN_EXT_GPIO_Port, LED_GREEN_EXT_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_RED_SMALL_GPIO_Port, LED_RED_SMALL_Pin, GPIO_PIN_RESET);
}

void turn_on_odd_leds(void){
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_GREEN_EXT_GPIO_Port, LED_GREEN_EXT_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_RED_SMALL_GPIO_Port, LED_RED_SMALL_Pin, GPIO_PIN_SET);
}

void led_effect_stop(void){
	for (int i=0; i<4; i++)
		xTimerStop(handle_led_timer[i], portMAX_DELAY);
	turn_off_all_leds();
}

void led_effect(int mode){

	turn_off_all_leds();
	xTimerStart(handle_led_timer[mode-1], portMAX_DELAY);
}

void LED_effect1(void){
	static int flag =1;
	//since flag is static, it keeps the value across function calls,
	//then XOR flips the bit every time
	(flag ^= 1) ? turn_off_all_leds() : turn_on_all_leds();
}

void LED_effect2(void){
	static int flag =1;
	(flag ^= 1) ? turn_on_even_leds() : turn_on_odd_leds();
}

void LED_effect3(void){
	static int count =0;
	count %= 5;
	count++;

	switch (count){
	case 1:
		HAL_GPIO_WritePin(LED_RED_SMALL_GPIO_Port, LED_RED_SMALL_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
		break;
	case 2:
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
		break;
	case 3:
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_SET);
		break;
	case 4:
		HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_GREEN_EXT_GPIO_Port, LED_GREEN_EXT_Pin, GPIO_PIN_SET);
		break;
	case 5:
		HAL_GPIO_WritePin(LED_GREEN_EXT_GPIO_Port, LED_GREEN_EXT_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_RED_SMALL_GPIO_Port, LED_RED_SMALL_Pin, GPIO_PIN_SET);
		break;
	}


}

void LED_effect4(void){
	static int count =0;
	count %=5;
	count++;

	switch (count){
	case 1:
		HAL_GPIO_WritePin(LED_RED_SMALL_GPIO_Port, LED_RED_SMALL_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
		break;
	case 2:
		HAL_GPIO_WritePin(LED_GREEN_EXT_GPIO_Port, LED_GREEN_EXT_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_RED_SMALL_GPIO_Port, LED_RED_SMALL_Pin, GPIO_PIN_RESET);
		break;
	case 3:
		HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_GREEN_EXT_GPIO_Port, LED_GREEN_EXT_Pin, GPIO_PIN_RESET);
		break;
	case 4:
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET);
		break;
	case 5:
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
		break;
	}
}

