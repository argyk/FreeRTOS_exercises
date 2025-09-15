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
	    "\r\n\r\n=========================\r\n"
	    "|         Menu          |\r\n"
	    "=========================\r\n"
	    "Please select an option:\r\n"
	    "Led Effect ---------> 0\r\n"
	    "Date and Time ------> 1\r\n"
	    "Exit ---------------> 2\r\n"
	    "Enter your choice here : ";

	const char* msg_invalid =
	    "\r\n\r\n=========================\r\n"
	    "|    Invalid message    |\r\n"
	    "=========================\r\n"
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
	const char* led_menu =
	    "\r\n\r\n=========================\r\n"
	    "|       LED Effect      |\r\n"
	    "=========================\r\n"
	    "Please select an option:\r\n"
	    "------> none\r\n"
	    "------> e1\r\n"
	    "------> e2\r\n"
	    "------> e3\r\n"
	    "------> e4\r\n"
		"Press any other key to exit.\r\n"
	    "Enter your choice here : ";
	const char* msg_led_invalid =
	    "\r\n\r\n=========================\r\n"
	    "|    Invalid message    |\r\n"
	    "=========================\r\n"
	    "Going back to main menu.\r\n";

	uint32_t cmd_addr;
	command_t* cmd;

	while(1){
		//Wait notify
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
		//print LED menu
		xQueueSendToBack(q_print, &led_menu, portMAX_DELAY);
		// wait for command
		xTaskNotifyWait(0,0,&cmd_addr,portMAX_DELAY);
		cmd = (command_t*) cmd_addr;

		//Select effect

		if (cmd->len <= 4){
			if (!strcmp((char*)cmd->payload, "none")){  //returns 0 if strings are equal
				led_effect_stop();
			}else if (!strcmp((char*)cmd->payload, "e1")){
				led_effect(1);
			}else if (!strcmp((char*)cmd->payload, "e2")){
				led_effect(2);
			}else if (!strcmp((char*)cmd->payload, "e3")){
				led_effect(3);
			}else if (!strcmp((char*)cmd->payload, "e4")){
				led_effect(4);
			}else{
				xQueueSend(q_print, &msg_led_invalid, portMAX_DELAY);
			}
		}else {
			xQueueSend(q_print, &msg_led_invalid, portMAX_DELAY);
		}
		currState = sMainMenu;
		xTaskNotify(handle_menu_task,0,eNoAction);

	}
}

void rtc_task(void *param){
	const char* rtc_menu_1 =
	    "\r\n\r\n"
		"=========================\r\n"
	    "|          RTC          |\r\n"
	    "=========================\r\n";
	const char* rtc_menu_2 =
	    "\r\n\r\n"
	    "Please select an option:\r\n"
	    "Set time ------------> 0\r\n"
	    "Set date ------------> 1\r\n"
	    "Show date and time --> 2\r\n"
		"Exit ----------------> 3\r\n"
	    "Enter your choice here : ";

	const char* msg_rtc_invalid =
	    "\r\n\r\n"
		"=========================\r\n"
	    "|    Invalid message    |\r\n"
	    "=========================\r\n"
	    "Going back to main menu.\r\n";

	const char* msg_set_time =
	    "\r\n\r\n"
		"=========================\r\n"
	    "|    Time configure     |\r\n"
	    "=========================\r\n"
	    "Please insert the time.\r\n";

	const char* msg_set_time_HH =
	    "\r\n"
	    "Hours (HH): \r\n";
	const char* msg_set_time_MM =
	    "\r\n"
	    "Minutes (MM): \r\n";
	const char* msg_set_time_SS =
	    "\r\n"
	    "Seconds (SS): \r\n";
	const char* msg_set_time_format =
	    "\r\n"
	    "Insert 0 for AM and 1 for PM: \r\n";

	const char* msg_set_date =
	    "\r\n\r\n"
		"=========================\r\n"
	    "|    Date configure     |\r\n"
	    "=========================\r\n"
	    "Please insert the date.\r\n";

	const char* msg_set_date_DD =
	    "\r\n"
	    "Day (DD): \r\n";
	const char* msg_set_date_MM =
	    "\r\n"
	    "Month (MM): \r\n";
	const char* msg_set_date_YYYY =
	    "\r\n"
	    "Year (YYYY): \r\n";

	uint32_t cmd_addr;
	command_t* cmd;
	uint8_t option;



	while(1){
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
		//print RTC menu
		xQueueSendToBack(q_print, &rtc_menu_1, portMAX_DELAY);
		show_time_date();
		xQueueSendToBack(q_print, &rtc_menu_2, portMAX_DELAY);



		while (currState != sMainMenu){
			// wait for command
			xTaskNotifyWait(0,0,&cmd_addr,portMAX_DELAY);
			cmd = (command_t*) cmd_addr;

			switch (currState){
			case (sRtcMenu):
				if (cmd->len ==1){
					option = cmd->payload[0] -48; //Convert ASCII to number

					switch(option){
					case 0:
						currState = sRtcTimeConfig;
						xQueueSendToBack(q_print, &msg_set_time, portMAX_DELAY);
						xQueueSendToBack(q_print, &msg_set_time_HH, portMAX_DELAY);
						break;
					case 1:
						currState = sRtcDateConfig;
						xQueueSendToBack(q_print, &msg_set_date, portMAX_DELAY);
						xQueueSendToBack(q_print, &msg_set_date_DD, portMAX_DELAY);
						break;
					case 2:
						currState = sRtcReport;
						break;
					case 3:
						currState = sMainMenu;
						break;
					default:
						currState = sMainMenu;
						xQueueSendToBack(q_print, &msg_rtc_invalid, portMAX_DELAY);

					}
				}
				else {
					xQueueSendToBack(q_print, &msg_rtc_invalid, portMAX_DELAY);

				}

				break;
			case (sRtcTimeConfig):
				break;
			case (sRtcDateConfig):
				break;
			case (sRtcReport):
				break;
			default:
				xQueueSendToBack(q_print, &msg_rtc_invalid, portMAX_DELAY);
				break;
			}



		}

		currState = sMainMenu;
		xTaskNotify(handle_menu_task,0,eNoAction);

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
