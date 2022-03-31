#ifndef _CONNECT_H_
#define _CONNECT_H_
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"
#include "esp_event_loop.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"

SemaphoreHandle_t binSemaphore;

void wifiInit();


#endif