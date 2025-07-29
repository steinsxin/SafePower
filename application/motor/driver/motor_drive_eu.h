//
// Created by Steins_Xin on 2025/7/29.
//

#ifndef SAFEPOWER_MOTOR_DRIVE_EU_H
#define SAFEPOWER_MOTOR_DRIVE_EU_H

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "gpio.h"
#include "can.h"

/* 应用层 */
#include "motor_config.h"
#include "can_handle.h"

/* ==================== 函数声明 ==================== */

/**
 * 发送获取电机角度命令
 * @param CAN_Bus CAN总线
 * @param CAN_ID 电机ID
 */
void EU_Get_Angle_CMD(CAN_HandleTypeDef *CAN_Bus, uint32_t CAN_ID);

/**
 * 发送电机速度控制命令
 * @param CAN_Bus CAN总线
 * @param CAN_ID 电机ID
 * @param speedControl 速度控制值
 */
void EU_Speed_CMD(CAN_HandleTypeDef *CAN_Bus, uint32_t CAN_ID, int32_t speedControl);

#endif //SAFEPOWER_MOTOR_DRIVE_EU_H
