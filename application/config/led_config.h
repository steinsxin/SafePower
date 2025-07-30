//
// Created by Steins_Xin on 2025/7/30.
//

#ifndef SAFEPOWER_LED_CONFIG_H
#define SAFEPOWER_LED_CONFIG_H

#include "gpio.h"

// 别名映射
#define LED1_GPIO_Port  LED_PA3_GPIO_Port
#define LED1_Pin        LED_PA3_Pin

#define LED2_GPIO_Port  LED_PA2_GPIO_Port
#define LED2_Pin        LED_PA2_Pin

#define LED3_GPIO_Port  LED_PC10_GPIO_Port
#define LED3_Pin        LED_PC10_Pin

#define LED4_GPIO_Port  LED_PC7_GPIO_Port
#define LED4_Pin        LED_PC7_Pin

#define LED5_GPIO_Port  LED_PC6_GPIO_Port
#define LED5_Pin        LED_PC6_Pin

// LED控制宏
#define LED_ON(led)        HAL_GPIO_WritePin(led##_GPIO_Port, led##_Pin, GPIO_PIN_SET)
#define LED_OFF(led)       HAL_GPIO_WritePin(led##_GPIO_Port, led##_Pin, GPIO_PIN_RESET)
#define LED_TOGGLE(led)    HAL_GPIO_TogglePin(led##_GPIO_Port, led##_Pin)

#endif //SAFEPOWER_LED_CONFIG_H
