#ifndef __MAIN_H__
#define __MAIN_H__

#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "driver/i2c.h"
#include "i2c-lcd.h"
#include "driver/uart.h"
#include "ultrasonic.h"
#include "dht_espidf.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_http_server.h"
#include "esp_http_client.h"
#include "esp_mac.h"


static const char *TAG = "MQTT_EXAMPLE";
#define ESP_WIFI_AP_SSID        "ESP32"
#define TOPIC_LED_FAN           "esp/led_fan"
#define TOPIC_DOOR              "esp/door"
#define TOPIC_OTHER_DEVICE      "esp/devices"
#define TOPIC_DHT               "esp/dht11"
#define TOPIC_TEST              "esp/test"
#define TOPIC_INIT              "esp/init"
#define TOPIC_DOOR_STATUS       "esp/door/status" 
#define TOPIC_GAS_SENSOR        "esp/gas_sensor"
#define TOPIC_CONFIG_WIFI       "esp/config_wifi"
#define TOPIC_CHANGE_MODE       "esp/change_wifi_mode"
#define ALERT_THRESHOLD         3000  

#define I2C_MASTER_SCL_IO       GPIO_NUM_22
#define I2C_MASTER_SDA_IO       GPIO_NUM_21
#define I2C_MASTER_NUM          0
#define I2C_MASTER_FREQ_HZ      400000
#define I2C_MASTER_TIMEOUT_MS   1000

#define UART_RX_PIN             GPIO_NUM_16
#define UART_TX_PIN             GPIO_NUM_17
#define UART_BUFFER             1024

#define TRIG_PIN                GPIO_NUM_5
#define ECHO_PIN                GPIO_NUM_18

#define ADC_PIN                 ADC1_CHANNEL_4  

#define DHT11_PIN               GPIO_NUM_19
#define BUZZER_PIN              GPIO_NUM_12

#define ESP_INTR_FLAG_DEFAULT   0

static esp_adc_cal_characteristics_t adc1_chars;
uint8_t mode_door = 0;

uint32_t MQTT_CONNEECTED = 0;
TaskHandle_t ultrasonic_task = NULL;
QueueHandle_t gpio_evt_queue = NULL;
esp_mqtt_client_handle_t client = NULL;
httpd_handle_t server = NULL;

esp_netif_t *esp_netif_sta;
esp_netif_t *esp_netif_ap;
static int s_retry_num = 0;

ultrasonic_senser_t ultrasonic = {
    .trig_pin = TRIG_PIN,
    .echo_pin = ECHO_PIN,
};
struct dht_reading dht11 = {
    .humidity = 0,
    .temperature = 0,
};
uint32_t gas_sensor = 0;

char _ssid[50] = "LiB", _password[100] = "123456789";

static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data);

void wifi_init(void);
void station_start();
void ap_start();

httpd_handle_t start_server(void);
esp_err_t post_handler(httpd_req_t *req);
esp_err_t get_handler(httpd_req_t *req);
esp_err_t get_sensor_handler(httpd_req_t *req);

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
static void mqtt_app_start(void);

void Publisher_Task(void *params);
void Ultrasonic_task();
void GasSensor_task();

void send_message(char *data);
void control_subscrtibe_message(char *topic, int topic_len, char *data, int data_len);

esp_err_t i2c_master_init();
void uart_init();
void adc_init();
void init_peripherals();

void read_storage();
void write_storage();
#endif