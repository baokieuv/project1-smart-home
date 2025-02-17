/*
 * led_fan.h
 *
 *  Created on: Dec 19, 2024
 *      Author: admin
 */

#ifndef INC_LED_FAN_H_
#define INC_LED_FAN_H_

#include "main.h"

#define BRIGHTNESS_MIN		0
#define BRIGHTNESS_MAX		255


typedef struct{
	uint8_t 		id;
	uint16_t 		brightness;
	TIM_TypeDef 	*timer;
	uint32_t 		channel;
	GPIO_TypeDef	*gpio_port;
	uint16_t 		gpio_pin;
}Led_Fan;

void Control_LED_FAN(Led_Fan *device);

#endif /* INC_LED_FAN_H_ */
