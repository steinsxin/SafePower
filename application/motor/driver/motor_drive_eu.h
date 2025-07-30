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
#include "can_handle.h"

// ==================== 命令 ID ====================

/**
 * @brief EU 电机命令枚举
 */
typedef enum {
    EU_CMD_GET_ANGLE     = 0x03,
    EU_CMD_SPEED_CONTROL = 0x05,
} EU_CMD_ID;

/* ==================== 函数声明 ==================== */

/**
 * @brief 发送读取多圈角度命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void EU_GetMultiTurnAngle(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID);

/**
 * @brief 发送读取单圈角度命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void EU_GetSingleTurnAngle(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID);

/**
 * @brief 发送读取状态1与错误标志命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void EU_GetStatus1AndError(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID);

/**
 * @brief 发送清除错误标志命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void EU_ClearErrorFlag(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID);

/**
 * @brief 发送转矩开环控制命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 * @param torqueValue 转矩值
 */
void EU_TorqueOpenLoop(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID, int16_t torqueValue);

/**
 * @brief 发送转矩闭环控制命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 * @param torqueValue 转矩值
 */
void EU_TorqueClosedLoop(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID, int16_t torqueValue);

/**
 * @brief 发送电机速度控制命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 * @param speedControl 速度值
 */
void EU_SpeedControl(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID, int32_t speedControl);

/**
 * @brief 发送电机停止命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void EU_StopMotor(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID);

/**
 * @brief 发送电机关闭命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void EU_DisableMotor(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID);

/**
 * @brief 发送电机使能（运行）命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void EU_EnableMotor(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID);

#endif //SAFEPOWER_MOTOR_DRIVE_EU_H
