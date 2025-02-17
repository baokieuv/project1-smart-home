/*
 * led_fan.c
 *
 *  Created on: Dec 19, 2024
 *      Author: admin
 */



#include "led_fan.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

//void Init_LED_FAN(char *detail[], Led_Fan *device){
//	device->id = atoi(detail[1]);
//
//	convert_gpio(&device->timer, &device->channel, &device->gpio_port, &device->gpio_pin, detail[2]);
//	LED_FAN_MspInit(device);
//}
void Control_LED_FAN(Led_Fan *device){
	if(device->timer == TIM1){
		__HAL_TIM_SET_COMPARE(&htim1, device->channel, device->brightness);
	}
	else if(device->timer == TIM2){
		__HAL_TIM_SET_COMPARE(&htim2, device->channel, device->brightness);
	}
	else{
		__HAL_TIM_SET_COMPARE(&htim3, device->channel, device->brightness);
	}
}

