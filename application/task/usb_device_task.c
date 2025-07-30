//
// Created by Steins_Xin on 2025/3/31.
//
#include "usb_device_task.h"

void usb_device_task(void const * argument){
    LED_ON(LED2);
    while (1){
        vTaskDelay(1000);
        LED_TOGGLE(LED2);
    }
}