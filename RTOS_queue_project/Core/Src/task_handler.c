/*
 * task_handler.c
 *
 *  Created on: Sep 10, 2025
 *      Author: argyris
 */

#include "task_handler.h"



void menu_task(void *param){

	uint32_t cmd_addr;
	command_t* cmd;
	int option;

	const char* msg_menu =
	    "=========================\r\n"
	    "|         Menu          |\r\n"
	    "=========================\r\n"
	    "Please select an option:\r\n"
	    "Led Effect ---------> 0\r\n"
	    "Date and Time ------> 1\r\n"
	    "Exit ---------------> 2\r\n"
	    "Enter your choice here : ";

	const char* msg_invalid =
	    "=========================\r\n"
	    "|         Menu          |\r\n"
	    "=========================\r\n"
	    "Invalid message !!\r\n"
	    "Please select option 0, 1 or 2.\r\n";

	while(1){
		xQueueSend(q_print, &msg_menu, portMAX_DELAY);

		xTaskNotifyWait(0,0,&cmd_addr,portMAX_DELAY);
		cmd = (command_t*) cmd_addr;

		if (cmd->len ==1){
			option = cmd->payload[0] -48; //Convert ASCII to number

			switch(option){
			case 0:
				currState = sLedEffect;
				xTaskNotify(handle_led_task,0,eNoAction);
				break;
			case 1:
				currState = sRtcMenu;
				xTaskNotify(handle_rtc_task,0,eNoAction);
				break;
			case 2:
				break;
			}
		}
		else {
			xQueueSend(q_print, &msg_invalid, portMAX_DELAY);

		}

		//Block until notified again
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);


	}
}

void led_task(void *param){
//	const char* msg_led =
//	    "=========================\r\n"
//	    "|       LED Effect       |\r\n"
//	    "=========================\r\n"
//	    "Please select an option:\r\n"
//	    "------> none\r\n"
//	    "------> e1\r\n"
//	    "------> e2\r\n"
//	    "------> e3\r\n"
//	    "------> e4\r\n"
//	    "Enter your choice here : ";

//	uint8_t data;
//	char output[50];

	while(1){

//		HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
//		vTaskDelay(500);
//   	    HAL_UART_Transmit(&huart2, (uint8_t*) msg_led, strlen(msg_led), HAL_MAX_DELAY);
//   	    HAL_UART_Receive(&huart2, &data, 1, HAL_MAX_DELAY);
//
//   	    snprintf(output, sizeof(output), "\r\n\r\nReceived char: %c !!\r\n Very nice !\r\n\r\n", data);
//
//   	    HAL_UART_Transmit(&huart2, (uint8_t*) output, strlen(output), HAL_MAX_DELAY);
//   	    strcpy(output, "");
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);

	}
}

void rtc_task(void *param){
	while(1){
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);

	}
}

void print_task(void *param){

	uint32_t *msg;
	while(1){
		xQueueReceive(q_print, &msg, portMAX_DELAY);
   	    HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen((char*)msg), HAL_MAX_DELAY);

	}
}




void cmd_handler_task(void *param){

	BaseType_t ret;
	command_t cmd;

	while(1){

		ret = xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
		if (ret==pdTRUE){
			process_command(&cmd);
		}

	}
}


void process_command(command_t *cmd){
	extract_command(cmd);

	switch(currState){
	case sMainMenu:
		xTaskNotify(handle_menu_task, (uint32_t)cmd, eSetValueWithoutOverwrite);
		break;
	case sLedEffect:
		xTaskNotify(handle_led_task, (uint32_t)cmd, eSetValueWithoutOverwrite);
		break;
	case sRtcMenu:
	case sRtcTimeConfig:
	case sRtcDateConfig:
	case sRtcReport:
		xTaskNotify(handle_rtc_task, (uint32_t)cmd, eSetValueWithoutOverwrite);

		break;
	}


}

int extract_command(command_t *cmd){

	uint8_t element;
	BaseType_t status;

	status = uxQueueMessagesWaiting(q_data);
	if (!status) return -1;
	uint8_t i=0;

	do {
		status = xQueueReceive(q_data, &element, portMAX_DELAY);
		if (status == pdTRUE) cmd->payload[i++] = element;
	}while (element != '\n');

	cmd->payload[i-1] = '\0';
	cmd->len = i-1;
	return 0;



}
