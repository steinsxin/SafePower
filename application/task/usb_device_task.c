//
// Created by Steins_Xin on 2025/3/31.
//
#include "usb_device_task.h"

void usb_device_task(void const * argument){
    LED_ON(LED2);
    while (1){
        printf("Hello SteinsXin STM32 HS\r\n");
        vTaskDelay(1000);
    }
}