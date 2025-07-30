//
// Created by Steins_Xin on 2025/3/31.
//
#include "usb_device_task.h"

void usb_device_task(void const * argument){
    LED_ON(LED2);
    while (1){
        printf("Hello %s STM32 HS\r\n", "SteinsXin");
        vTaskDelay(1000);
        LED_TOGGLE(LED2);
    }
}