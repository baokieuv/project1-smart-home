/*
 * Copyright (c) 2016 Ruslan V. Uss <unclerus@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of itscontributors
 *    may be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file ultrasonic.c
 *
 * ESP-IDF driver for ultrasonic range meters, e.g. HC-SR04, HY-SRF05 and the like
 *
 * Ported from esp-open-rtos
 *
 * Copyright (c) 2016 Ruslan V. Uss <unclerus@gmail.com>
 *
 * BSD Licensed as described in the file LICENSE
 */
//#include <esp_idf_lib_helpers.h>
#include "ultrasonic.h"

esp_err_t ultrasonic_init(ultrasonic_senser_t *dev){
    esp_err_t err = gpio_set_direction(dev->trig_pin, GPIO_MODE_OUTPUT);
    if(err != ESP_OK) return err;
    err = gpio_set_direction(dev->echo_pin, GPIO_MODE_INPUT);
    if(err != ESP_OK) return err;
    err = gpio_set_level(dev->trig_pin, 1);

    return err;
}

esp_err_t ultrasonic_measure(ultrasonic_senser_t *dev, uint32_t *dis){
    gpio_set_level(dev->trig_pin, 0);
    esp_rom_delay_us(2);
    gpio_set_level(dev->trig_pin, 1);
    esp_rom_delay_us(10);
    gpio_set_level(dev->trig_pin, 0);

    int64_t start_time = esp_timer_get_time();
    int64_t prev_time = start_time;

    while(gpio_get_level(dev->echo_pin) == 0){
        start_time = esp_timer_get_time();
        if(start_time - prev_time > ECHO_TIMEOUT){
            ESP_LOGI("HC_SR04", "Timeout 1");
            return ESP_ERR_TIMEOUT;
        }
    }
    
    int64_t end_time = start_time;
    while (gpio_get_level(dev->echo_pin) == 1) {
        end_time = esp_timer_get_time();
        if(end_time - start_time > ECHO_TIMEOUT){
            ESP_LOGI("HC_SR04", "Timeout 2");
            return ESP_ERR_TIMEOUT;
        }
    }
    gpio_set_level(dev->trig_pin, 1);
    int64_t duration = end_time - start_time; // Thời gian phản hồi trong microseconds
    *dis = (duration / 2.0) * (SOUND_SPEED / 10000.0); // Khoảng cách (m)

    return ESP_OK;
}