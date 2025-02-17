/* ****************************************************************************
 *
 * ESP32 platform interface for DHT temperature & humidity sensors
 *
 * Copyright (c) 2017, Arnim Laeuger
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * ****************************************************************************/


#ifndef __DHT_ESPIDF_H
#define __DHT_ESPIDF_H

// ---------------------------------------------------------------------------
// INCLUDES

#include <stdint.h>

#include "sdkconfig.h"

#include <esp_err.h>
#include <driver/gpio.h>

/*** 
 * 
 * Types
 **/

/** @brief  Lưu trữ kết của nhiệt độ, độ ẩm khi đọc từ cảm biến */
struct dht_reading {
    double humidity;      //lưu độ ẩm
    double temperature;   //lưu nhiệt độ
};

/** @brief  Các trạng thái có thể trả về của hàm*/
typedef enum {
  DHT_OK = 0,                 //Thành công
  DHT_ERROR_CHECKSUM = -1,    //Lỗi do checksum sai
  DHT_ERROR_TIMEOUT = -2,     //Lỗi do quá thời gian
  DHT_INVALID_VALUE = -999    //Lỗi do giá trị không hợp lệ
} dht_result_t;

/** @brief  Các loại cảm biến*/
typedef enum {
  DHT11,
  DHT2X
} dht_type_t;


/** 
  * @brief   Điều khiển cảm biến DHTxx sau đó nhận kết quả đo được từ cảm biến
  * @param gpio_num      chân dữ liệu + điều khiển DHTxx (tạo ra các xung + đo mức điện áp -> nhận dữ liệu) 
  * @param type          loại cảm biến DHTxx (DHT11, DHT2x)
  * @param sensor_data   dùng để lưu kết quả nhiệt độ và độ ẩm nhận được từ cảm biến
  * @retval  Kết quả của việc đọc cảm biến (thành công or error)
  */
dht_result_t read_dht_sensor_data(const gpio_num_t gpio_num, dht_type_t type, struct dht_reading *sensor_data);

#endif
