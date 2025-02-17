/*
 * other_devices.c
 *
 *  Created on: Dec 19, 2024
 *      Author: admin
 */


#include "other_devices.h"

//void Init_Device(char *detail[], Oher_Devices *device){
//	device->id = atoi(detail[1]);
//
//	convert_gpio(NULL, NULL, &device->gpio_port, &device->gpio_pin, detail[2]);
//	Device_MspInit(device);
//}
void Control_Device(Oher_Devices *device){
	if(device->status == 1){
		HAL_GPIO_WritePin(device->gpio_port, device->gpio_pin, ENABLE);
	}
	else{
		HAL_GPIO_WritePin(device->gpio_port, device->gpio_pin, DISABLE);
	}
}

