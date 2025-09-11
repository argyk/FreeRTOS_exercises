/*
 * task_handler.h
 *
 *  Created on: Sep 10, 2025
 *      Author: argyris
 */

#ifndef INC_TASK_HANDLER_H_
#define INC_TASK_HANDLER_H_

#include "main.h"

void led_task(void *param);
void menu_task(void *param);
void rtc_task(void *param);
void print_task(void *param);
void cmd_handler_task(void *param);

#endif /* INC_TASK_HANDLER_H_ */
