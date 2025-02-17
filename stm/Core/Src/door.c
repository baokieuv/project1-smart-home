/*
 * door.c
 *
 *  Created on: Dec 19, 2024
 *      Author: admin
 */



#include "door.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

//void Init_DOOR(char *detail[], Door *door){
//	door->id = atoi(detail[1]);
//
//	convert_gpio(&door->timer, &door->channel, &door->gpio_port, &door->gpio_pin, detail[2]);
//	DOOR_MspInit(door);
//}
void Control_DOOR(Door *door){
	if(door->status == STATUS_ON){
		if(door->timer == TIM1){
			__HAL_TIM_SET_COMPARE(&htim1, door->channel, 250);
		}
		else if(door->timer == TIM2){
			__HAL_TIM_SET_COMPARE(&htim2, door->channel, 250);
		}
		else{
			__HAL_TIM_SET_COMPARE(&htim3, door->channel, 250);
		}
	}else{
		if(door->timer == TIM1){
			__HAL_TIM_SET_COMPARE(&htim1, door->channel, 50);
		}
		else if(door->timer == TIM2){
			__HAL_TIM_SET_COMPARE(&htim2, door->channel, 50);
		}
		else{
			__HAL_TIM_SET_COMPARE(&htim3, door->channel, 50);
		}
	}
}

