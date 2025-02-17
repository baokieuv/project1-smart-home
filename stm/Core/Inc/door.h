/*
 * door.h
 *
 *  Created on: Dec 19, 2024
 *      Author: admin
 */

#ifndef INC_DOOR_H_
#define INC_DOOR_H_

#include "main.h"

#define STATUS_ON	1
#define STATUS_OFF	0

typedef struct{
	uint8_t 		id;
	uint8_t 		status;
	TIM_TypeDef 	*timer;
	uint32_t 		channel;
	GPIO_TypeDef	*gpio_port;
	uint16_t 		gpio_pin;
}Door;

void Control_DOOR(Door *device);

#endif /* INC_DOOR_H_ */
