#include "main.h"

#define MIN(a, b) (a) < (b) ? (a) : (b)

static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data)
{
    switch(event_id){
        case WIFI_EVENT_AP_STACONNECTED:
            wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *) event_data;
            ESP_LOGI(TAG, "Station "MACSTR" joined, AID = %d",
                 MAC2STR(event->mac), event->aid);
            break;
        case WIFI_EVENT_AP_STADISCONNECTED:
            wifi_event_ap_stadisconnected_t *event1 = (wifi_event_ap_stadisconnected_t *) event_data;
            ESP_LOGI(TAG, "Station "MACSTR" left, AID = %d, reason: %d",
                 MAC2STR(event1->mac), event1->aid, event1->reason);
            break;
        case WIFI_EVENT_STA_START:
            esp_wifi_connect();
            break;
        case WIFI_EVENT_STA_CONNECTED:
            ESP_LOGI(TAG, "Wi-Fi connected\n");
            break;
        case IP_EVENT_STA_GOT_IP:
            ESP_LOGI(TAG, "got ip: starting MQTT Client\n");
            s_retry_num = 0;
            if(client != NULL){
                //esp_mqtt_client_disconnect(client);
                MQTT_CONNEECTED=0;
            }
            mqtt_app_start();
            break;
        case WIFI_EVENT_STA_DISCONNECTED:
            if(s_retry_num < 5){
                ESP_LOGI(TAG, "disconnected: Retrying Wi-Fi\n");
                esp_wifi_connect();
                s_retry_num++;
            }else{
                ESP_LOGI(TAG,"Connect to the AP fail");
                s_retry_num = 0;
                ap_start();
            }
            break;
        default:
            break;
    }
}
void wifi_init(void)
{
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_sta = esp_netif_create_default_wifi_sta();
    esp_netif_ap = esp_netif_create_default_wifi_ap();

    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

}
void station_start(){
    ESP_LOGI(TAG, "Station mode start!");
    ESP_ERROR_CHECK(esp_wifi_stop());
    // if(server != NULL){
    //     httpd_stop(server);
    //     server = NULL;
    // }
    s_retry_num = 0;

    esp_netif_set_default_netif(esp_netif_sta);
    wifi_config_t wifi_config = {
        .sta = {
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
        }
    };
    strcpy((char*)wifi_config.sta.ssid, _ssid);
    strcpy((char*)wifi_config.sta.password, _password);

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start());
    esp_wifi_connect();

    // lcd_clear();
    // lcd_put_cur(0, 0);
    // lcd_send_string("Welcome Home");
    // lcd_put_cur(1, 0);
    // lcd_send_string("Wi-Fi: Sation");
}
void ap_start(){
    ESP_LOGI(TAG, "AP mode start!");

    if(client != NULL){
        //esp_mqtt_client_disconnect(client);
        MQTT_CONNEECTED = 0;
    }

    ESP_ERROR_CHECK(esp_wifi_stop());
    esp_netif_set_default_netif(esp_netif_ap);
    wifi_config_t wifi_config = {
        .ap = {
            .ssid = ESP_WIFI_AP_SSID,
            .ssid_len = strlen(ESP_WIFI_AP_SSID),
            .max_connection = 4,
            .authmode = WIFI_AUTH_OPEN,
        },
    };

    esp_wifi_set_mode(WIFI_MODE_AP);
    esp_wifi_set_config(WIFI_IF_AP, &wifi_config);
    esp_wifi_start();
    start_server();

    // lcd_clear();
    // lcd_put_cur(0, 0);
    // lcd_send_string("Welcome Home");
    // lcd_put_cur(1, 0);
    // lcd_send_string("Wi-Fi: AP");
}

// Setup URI handlers
httpd_handle_t start_server(void) {
    if(server == NULL){
        httpd_config_t config = HTTPD_DEFAULT_CONFIG();

        if (httpd_start(&server, &config) == ESP_OK) {
            httpd_uri_t post_uri = {
                .uri = "/post",
                .method = HTTP_POST,
                .handler = post_handler
            };
            httpd_register_uri_handler(server, &post_uri);

            httpd_uri_t get_uri = {
                .uri = "/get",
                .method = HTTP_GET,
                .handler = get_handler
            };
            httpd_register_uri_handler(server, &get_uri);

            httpd_uri_t get_sensor_uri = {
                .uri = "/get_sensor",
                .method = HTTP_GET,
                .handler = get_sensor_handler
            };
            httpd_register_uri_handler(server, &get_sensor_uri);
            ESP_LOGE(TAG, "Start server successfully!!!");
            return server;
        }else{
            ESP_LOGE(TAG, "Failed to start server");
            return NULL;
        }
    }else{
        ESP_LOGE(TAG, "Start server successfully!!!");
        return server;
    }

}
// HTTP POST handler - Nhận dữ liệu từ Android
esp_err_t post_handler(httpd_req_t *req){
    char buf[200];
    int ret, remaining = req->content_len;

    while (remaining > 0) {
        if ((ret = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf)))) <= 0) {
            if (ret == HTTPD_SOCK_ERR_TIMEOUT) continue;
            return ESP_FAIL;
        }
        remaining -= ret;
        buf[ret] = 0; // Null-terminate buffer
        ESP_LOGI(TAG, "Received: %s", buf);
    }

    char *topic = strtok(buf, " ");
    char *data = strtok(NULL, "@");
    control_subscrtibe_message(topic, strlen(topic), data, strlen(data));

    httpd_resp_send(req, "Data received", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}
// HTTP GET handler - Gửi dữ liệu tới Android
esp_err_t get_handler(httpd_req_t *req) {
    //const char *response = "Hello from ESP32";
    char response[100];
    sprintf(response, "%s %.1lf+%.1lf+%ld", TOPIC_DHT, dht11.temperature, dht11.humidity, gas_sensor);

    httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
    ESP_LOGI(TAG, "Sent: %s", response);
    return ESP_OK;
}
esp_err_t get_sensor_handler(httpd_req_t *req) {
    send_message("FIRST_INIT");
    uint8_t size = 0;
    char mess[200];
    int err = uart_read_bytes(UART_NUM_1, &size, 1, 1000/portTICK_PERIOD_MS);
    if(err == -1) return ESP_ERR_NOT_FINISHED;
    err = uart_read_bytes(UART_NUM_1, mess, size, 3000/portTICK_PERIOD_MS);
    if(err == -1) return ESP_ERR_NOT_FINISHED;
    printf("Data from stm32: %s\n", mess);

    httpd_resp_send(req, mess, HTTPD_RESP_USE_STRLEN);
    ESP_LOGI(TAG, "Sent: %s", mess);
    return ESP_OK;
}


void Publisher_Task(void *params)
{
  while (true)
  {
    read_dht_sensor_data(DHT11_PIN, DHT11, &dht11);
    char data[100];
    sprintf(data, "%.1lf %.1lf", dht11.temperature, dht11.humidity);
    printf("Temperature: %.1lf, Humidity: %.1lf\n", dht11.temperature, dht11.humidity);
    if(MQTT_CONNEECTED)
        esp_mqtt_client_publish(client, TOPIC_DHT, data, strlen(data), 0, 0);

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
void Ultrasonic_task(){
    while(1){
        uint32_t dis = 0xffffff;
        ultrasonic_measure(&ultrasonic, &dis);
        printf("Distance: %ld\n", dis);
        if(dis < 20){
            send_message("CONTROL+DOOR+5+OPEN");
            if(MQTT_CONNEECTED)
                esp_mqtt_client_publish(client, TOPIC_DOOR_STATUS, "OPEN", 4, 0, 0);
        }
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}
void GasSensor_task(){
    while(1){
        gas_sensor = 0;
        for(int i = 0; i < 10; i++){
            gas_sensor += adc1_get_raw(ADC_PIN);
        }
        gas_sensor /= 10;
        printf("Gas Sensor: %ld\n", gas_sensor);
        if(gas_sensor > 2000){
            gpio_set_level(BUZZER_PIN, 1);
        }
        gpio_set_level(BUZZER_PIN, 0);

        char data[10];
        sprintf(data, "%ld", gas_sensor);
        if(MQTT_CONNEECTED)
            esp_mqtt_client_publish(client, TOPIC_GAS_SENSOR, data, strlen(data), 0, 0);
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

void send_message(char *data){
    printf("Data send to stm32: %s\n", data);
    int size = strlen(data);
    uart_write_bytes(UART_NUM_1, &size, 1);
    vTaskDelay(10/portTICK_PERIOD_MS);
    uart_write_bytes(UART_NUM_1, data, size);
}
void control_subscrtibe_message(char *topic, int topic_len, char *data, int data_len){
    printf("%s %s\n", topic, data);
    if(strncmp(topic, TOPIC_LED_FAN, topic_len) == 0){
        char message[100];
        strcpy(message, "CONTROL+LED_FAN+");
        strncat(message, data, data_len);
        send_message(message);
    }
    else if(strncmp(topic, TOPIC_OTHER_DEVICE, topic_len) == 0){
        char message[100];
        strcpy(message, "CONTROL+DEVICES+");
        strncat(message, data, data_len);
        send_message(message);
    }
    else if(strncmp(topic, TOPIC_DOOR, topic_len) == 0){
        char tmp[100];
        strncpy(tmp, data, data_len);
        tmp[data_len] = '\0';
        
        char *token = strtok(data, "+");
        char *val = strtok(NULL, "+");
        if(val[0] == 'O' || val[0] == 'C'){
            char message[] = "CONTROL+DOOR+";
            strcat(message, tmp);
            send_message(message);
        }else{
            if(atoi(token) == 5){
                if(strncmp(val, "AUTO", 4) == 0){
                    printf("On Auto mode\n");
                    mode_door = 1;
                    if(ultrasonic_task == NULL)
                        xTaskCreate(Ultrasonic_task, "Ultrasonic_task", 2024, NULL, 5, &ultrasonic_task);
                }else{
                    mode_door = 0;
                    if(ultrasonic_task != NULL)
                        vTaskDelete(ultrasonic_task);
                    ultrasonic_task = NULL;
                }
            }
        }
    }
    else if(strncmp(topic, TOPIC_TEST, topic_len) == 0){
        if(strncmp(data, "INIT", data_len) == 0){
            send_message("FIRST_INIT");
            uint8_t size = 0;
            char mess[200];
            int err = uart_read_bytes(UART_NUM_1, &size, 1, 1000/portTICK_PERIOD_MS);
            if(err == -1) return;
            err = uart_read_bytes(UART_NUM_1, mess, size, 3000/portTICK_PERIOD_MS);
            if(err == -1) return;
            printf("Data from stm32: %s\n", mess);
            if(MQTT_CONNEECTED){
                esp_mqtt_client_publish(client, TOPIC_INIT, mess, strlen(mess), 0, 0);
                vTaskDelay(100/portTICK_PERIOD_MS);
                sprintf(mess, "5+%s+@", mode_door == 0 ? "NORMAL" : "AUTO");
                esp_mqtt_client_publish(client, TOPIC_INIT, mess, strlen(mess), 0, 0);
            }else{

            }
        }
    }
    else if(strncmp(topic, TOPIC_CONFIG_WIFI, topic_len) == 0){
        if(strncmp(data, "update", data_len) == 0){
            station_start();
        }
        else{
            char *ssid = strtok(data, " ");
            char *pass = strtok(NULL, " ");
            strcpy(_ssid, ssid);
            strcpy(_password, pass);
            printf("ssid: %s, pass: %s\n", _ssid, _password);
            write_storage();
        }
    }
    else if(strncmp(topic, TOPIC_CHANGE_MODE, topic_len) == 0){
        if(strncmp(data, "AP", data_len) == 0){
            ap_start();
        }else{
            station_start();
        }
    }
}
/*
 * @brief Event handler registered to receive MQTT events
 *
 *  This function is called by the MQTT client event loop.
 *
 * @param handler_args user data registered to the event.
 * @param base Event base for the handler(always MQTT Base in this example).
 * @param event_id The id for the received event.
 * @param event_data The data for the event, esp_mqtt_event_handle_t.
 */
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGI(TAG, "Event dispatched from event loop base=%s, event_id=%ld", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    if(client == NULL) return;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        MQTT_CONNEECTED=1;
        
        msg_id = esp_mqtt_client_subscribe(client, TOPIC_LED_FAN, 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_subscribe(client, TOPIC_DOOR, 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_subscribe(client, TOPIC_OTHER_DEVICE, 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_subscribe(client, TOPIC_TEST, 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_subscribe(client, TOPIC_CHANGE_MODE, 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_subscribe(client, TOPIC_CONFIG_WIFI, 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
        gpio_set_level(GPIO_NUM_2, 1);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        MQTT_CONNEECTED=0;
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        // lcd_clear();
        // lcd_put_cur(0, 0);
        // lcd_send_string("------HUST------");
        // lcd_put_cur(1, 0);
        // lcd_send_string("Welcome Home");
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        control_subscrtibe_message(event->topic, event->topic_len, event->data, event->data_len);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}
static void mqtt_app_start(void)
{
    ESP_LOGI(TAG, "STARTING MQTT");
    if(client == NULL){
        esp_mqtt_client_config_t mqttConfig = {
            .broker.address.uri = "mqtt://broker.hivemq.com:1883",
        };
        
        client = esp_mqtt_client_init(&mqttConfig);
        esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
        esp_mqtt_client_start(client);
    }else{
        MQTT_CONNEECTED = 1;
        //esp_mqtt_client_reconnect(client);
    }
}

esp_err_t i2c_master_init(){
    int i2c_master_port = I2C_MASTER_NUM;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(i2c_master_port, &conf);
    return i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0);
}
void uart_init(){
    uart_config_t conf = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
    };

    ESP_ERROR_CHECK(uart_param_config(UART_NUM_1, &conf));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_1, UART_TX_PIN, UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_1, UART_BUFFER, UART_BUFFER, 10, NULL, 0));
}
void adc_init(){
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_12, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);
    adc1_config_width(ADC_WIDTH_BIT_DEFAULT);
    adc1_config_channel_atten(ADC_PIN, ADC_ATTEN_DB_12);
}
void init_peripherals(){
    //ESP_ERROR_CHECK(i2c_master_init());
    uart_init();
    //lcd_init();
    adc_init();
    ultrasonic_init(&ultrasonic);
    gpio_set_direction(BUZZER_PIN, GPIO_MODE_OUTPUT);
}

void read_storage(){
    //int8_t flag = 1; //cờ đánh dấu đọc thành công (1 = thành công, 0 = không thành công)
    size_t size;
    nvs_handle_t my_handle;

    //mở NVS với namespace đã cho từ phân vùng NVS mặc định (namespace = storage) với chế độ chỉ đọc
    esp_err_t err = nvs_open("storage", NVS_READONLY, &my_handle);
    if(err != ESP_OK){
        ESP_LOGI(TAG, "Error %s opening nvs!", esp_err_to_name(err));
        nvs_close(my_handle);  
        return;
    }
    
    //lấy giá trị chuỗi cho khóa đã cho. Nếu key không tồn tại hoặc kiểu dữ liệu không 
    //khớp thì error sẽ được trả về. Trong trường hợp có lỗi thì out_value sẽ không được sửa đổi
    err = nvs_get_str(my_handle, "ssid", NULL, &size);
    err = nvs_get_str(my_handle, "ssid", _ssid, &size);
    if(err != ESP_OK){
       // flag = 0;
        ESP_LOGI(TAG, "Error %s reading ssid from nvs!", esp_err_to_name(err));
    }

    err = nvs_get_str(my_handle, "password", NULL, &size);
    err = nvs_get_str(my_handle, "password", _password, &size);
    if(err != ESP_OK){
       // flag = 0;
        ESP_LOGI(TAG, "Error %s reading password from nvs!", esp_err_to_name(err));
    }   

    //đóng storage handle và giải phóng toàn bộ tài nguyên đươc phân bổ
    nvs_close(my_handle);

    printf("Read done! ssid: %s, password: %s\n", _ssid, _password);
    //return flag;
}
void write_storage(){
    nvs_handle_t my_handle;

    //mở NVS với namespace đã cho từ phân vùng NVS mặc định (namespace = storage) với chế độ đọc+ghi
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK){
        ESP_LOGI(TAG, "Error %s opening nvs!", esp_err_to_name(err));
    } else {
        //set string cho key.  Việc lưu trữ xâu dài có thể bị lỗi do bị phân mảnh bộ nhớ
        //bộ nhớ sẽ không được cập nhật cho đến khi hàm nvs_commit() được gọi
        ESP_ERROR_CHECK(nvs_set_str(my_handle, "ssid", _ssid));
        ESP_ERROR_CHECK(nvs_set_str(my_handle, "password", _password));

        //ghi bất kỳ thay đổi nào đang chờ xử lý vào NVS
        ESP_ERROR_CHECK(nvs_commit(my_handle));
        printf("Write done! ssid: %s, password: %s\n", _ssid, _password);
    }

    //đóng storage handle và giải phóng toàn bộ tài nguyên đươc phân bổ
    nvs_close(my_handle); 
}

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    init_peripherals();

    // lcd_clear();
    // lcd_put_cur(0, 0);
    // lcd_send_string("------HUST------");
    
    // lcd_put_cur(1, 0);
    // lcd_send_string("Hello World");
    read_storage();
    wifi_init();
    station_start();
    start_server();

    xTaskCreate(Publisher_Task, "Publisher_Task", 2028, NULL, 5, NULL);
    xTaskCreate(GasSensor_task, "GasSensor_task", 2028, NULL, 5, NULL);
}