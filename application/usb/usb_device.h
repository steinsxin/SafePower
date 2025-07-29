//
// Created by Steins_Xin on 2025/3/31.
//

#ifndef SAFEPOWER_USB_DEVICE_TASK_H
#define SAFEPOWER_USB_DEVICE_TASK_H

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "gpio.h"

// USB驱动
#include "usb_device.h"
#include "usbd_cdc_if.h"

extern USBD_HandleTypeDef hUsbDeviceHS;
int USB_SendData(uint8_t* data, uint16_t len);

#endif //SAFEPOWER_USB_DEVICE_TASK_H
