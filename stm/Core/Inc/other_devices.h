/*
 * other_devices.h
 *
 *  Created on: Dec 19, 2024
 *      Author: admin
 */

#ifndef INC_OTHER_DEVICES_H_
#define INC_OTHER_DEVICES_H_

#include "main.h"

typedef struct{
	uint8_t 		id;
	uint8_t 		status;
	GPIO_TypeDef	*gpio_port;
	uint32_t 		gpio_pin;
}Oher_Devices;

void Control_Device(Oher_Devices *device);

#endif /* INC_OTHER_DEVICES_H_ */
