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
 * @file ultrasonic.h
 * @defgroup ultrasonic ultrasonic
 * @{
 *
 * ESP-IDF driver for ultrasonic range meters, e.g. HC-SR04, HY-SRF05 and so on
 *
 * Ported from esp-open-rtos
 *
 * Copyright (c) 2016 Ruslan V. Uss <unclerus@gmail.com>
 *
 * BSD Licensed as described in the file LICENSE
 */
#ifndef __ULTRASONIC_H__
#define __ULTRASONIC_H__

#include <driver/timer.h>
#include <driver/gpio.h>
#include "esp_timer.h"
#include "esp_log.h"

#define SOUND_SPEED     343.0
#define ECHO_TIMEOUT    300000

typedef struct {
    uint8_t trig_pin;
    uint8_t echo_pin;
}ultrasonic_senser_t;

esp_err_t ultrasonic_init(ultrasonic_senser_t *dev);

esp_err_t ultrasonic_measure(ultrasonic_senser_t *dev, uint32_t *dis);
#endif /* __ULTRASONIC_H__ */