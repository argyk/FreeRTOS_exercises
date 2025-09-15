/*
 * task_handler.h
 *
 *  Created on: Sep 10, 2025
 *      Author: argyris
 */

#ifndef INC_TASK_HANDLER_H_
#define INC_TASK_HANDLER_H_

#include "stm32f3xx.h"
#include "usart.h"
#include "rtc.h"
#include "main.h"

#include <string.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"



void led_task(void *param);
void menu_task(void *param);
void rtc_task(void *param);
void print_task(void *param);
void cmd_handler_task(void *param);

extern TaskHandle_t handle_led_task;
extern TaskHandle_t handle_rtc_task;
extern TaskHandle_t handle_menu_task;
extern TaskHandle_t handle_print_task;
extern TaskHandle_t handle_cmd_task;

extern QueueHandle_t q_print;
extern QueueHandle_t q_data;

extern TimerHandle_t handle_led_timer[4];

extern uint8_t userData;



typedef enum {
	sMainMenu =0,
	sLedEffect,
	sRtcMenu,
	sRtcTimeConfig,
	sRtcDateConfig,
	sRtcReport,
}state_t;

typedef struct {
	uint8_t payload[10];
	uint32_t len;
}command_t;


extern state_t currState;

void process_command(command_t *cmd);
int extract_command(command_t *cmd);

void led_effect_stop(void);
void led_effect(int n);

void turn_on_all_leds(void);
void turn_off_all_leds(void);
void turn_on_even_leds(void);
void turn_on_odd_leds(void);

void led_effect_callback(TimerHandle_t xTimer);
void LED_effect1(void);
void LED_effect2(void);
void LED_effect3(void);
void LED_effect4(void);


#endif /* INC_TASK_HANDLER_H_ */
