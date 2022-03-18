#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"
#include "esp_event_loop.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "cJSON.h"
#include "connect.h"
#include "mqtt_client.h"
#include "driver/gpio.h"

//#define SET_GPIO CONFIG_BLINK_GPIO
//#define SET_GPIO (gpio_num_t)

#define TAG "MQTT"
#define MAC "MAC"

xQueueHandle readingQueue;
TaskHandle_t taskHandle;

const uint32_t WIFI_CONNEECTED = BIT1;
const uint32_t MQTT_CONNECTED = BIT2;
const uint32_t MQTT_PUBLISHED = BIT3;

void mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
  switch (event->event_id)
  {
  case MQTT_EVENT_CONNECTED:
    ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
    xTaskNotify(taskHandle, MQTT_CONNECTED, eSetValueWithOverwrite);
    break;
  case MQTT_EVENT_DISCONNECTED:
    ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
    break;
  case MQTT_EVENT_SUBSCRIBED:
    ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
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
    
    // printf("DATA=%.*s\r\n", event->data_len, event->data);
    

    if (strncmp(event->topic, "/led", event->topic_len)==0) {
        cJSON *msg = cJSON_Parse(event->data);

        if(msg != NULL){
            cJSON *item = cJSON_GetObjectItem(msg, "action" );
            ESP_LOGI(TAG, "VALUE: %d", item->valueint);
            int valor;
            valor = item->valueint;
            if(valor==0){
                ESP_LOGI(TAG, "LED OFF");
                const int SET_GPIO = 2;
                gpio_pad_select_gpio(SET_GPIO);
                gpio_set_direction(SET_GPIO, GPIO_MODE_OUTPUT);
                gpio_set_level((gpio_num_t)SET_GPIO, 0);

            }
        
            else if(valor==1){
                ESP_LOGI(TAG, "LED ON");
     
                const int SET_GPIO = 2;
                gpio_pad_select_gpio(SET_GPIO);
                gpio_set_direction(SET_GPIO, GPIO_MODE_OUTPUT);
                gpio_set_level((gpio_num_t)SET_GPIO, 1);
            }
            else{
                ESP_LOGI(TAG, "INVALLID");
                esp_mqtt_client_handle_t client = event->client;
                uint8_t macAddr [6];
                esp_wifi_get_mac(0,macAddr);
                ESP_LOGI(MAC, "" MACSTR "", MAC2STR(macAddr));

                char data[50];
                sprintf(data," " MACSTR "", MAC2STR(macAddr));
                esp_mqtt_client_publish(client, "/error", data, strlen(data), 0, false);

            }
        }

    }
    break;
  case MQTT_EVENT_ERROR:
    ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
    break;
  default:
    ESP_LOGI(TAG, "Other event id:%d", event->event_id);
    break;
  }
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
  mqtt_event_handler_cb(event_data);
}

void task_manager(void *para)
{
  uint32_t command = 0;
  esp_mqtt_client_config_t mqttConfig = {
    //   .uri = "mqtt://test.mosquitto.org:1883"};
      .uri = "mqtt://mqtt.eclipseprojects.io"};
  esp_mqtt_client_handle_t client = NULL;

  while (true)
  {
    xTaskNotifyWait(0, 0, &command, portMAX_DELAY);
    switch (command)
    {
    case WIFI_CONNEECTED:
      client = esp_mqtt_client_init(&mqttConfig);
      esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
      esp_mqtt_client_start(client);
      break;
    case MQTT_CONNECTED:
      esp_mqtt_client_subscribe(client, "/led", 0);
    //   char data[50];
    //   sprintf(data, "%d", 1);
    //   esp_mqtt_client_publish(client, "/error", data, strlen(data), 0, false);

      break;
   
    default:
      break;
    }
  }
}

void app_main()
{

  wifiInit();
  xTaskCreate(task_manager, "manager", 1024 * 5, NULL, 5, &taskHandle);

}