//
// Created by Steins_Xin on 2025/4/8.
//
#include "BSP_TIM.h"

uint32_t Timer_Ticks = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM1) {  // 判断是 TIM1 的中断
        Timer_Ticks++;  // 每 1ms 增加一次
    }
}

/**
 *
 * @return
 */
uint32_t get_timer_ticks() {
    return Timer_Ticks;  // 返回当前的 ticks
}