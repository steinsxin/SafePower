//
// Created by Steins_Xin on 2025/3/31.
//
#include "USBDevice.h"


void usb_device_task(void const * argument){
    while (1){
        printf("Hello SteinsXin STM32 HS\r\n");
        printf("AWdawd %s","Hello");
        vTaskDelay(1000);
    }
}

/**
 * USB_SendData
 * @param data
 * @param len
 * @return
 */
int USB_SendData(uint8_t* data, uint16_t len) {
    if (hUsbDeviceHS.dev_state != USBD_STATE_CONFIGURED) return -1;

    USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceHS.pClassData;
    while (hcdc->TxState != 0) {}

    return (CDC_Transmit_HS(data, len) == USBD_OK) ? 0 : -2;
}

/**
 * printf 重定向
 * @param format
 * @param ...
 */
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE* f)
#endif


PUTCHAR_PROTOTYPE
{
    USB_SendData((uint8_t*)&ch, 1);
    return ch;
}