//
// Created by Steins_Xin on 2025/7/29.
//

#ifndef SAFEPOWER_MOTOR_DRIVE_LK_H
#define SAFEPOWER_MOTOR_DRIVE_LK_H

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include <string.h>
#include "can.h"

/* 应用层 */
#include "can_handle.h"

#define LK_TORQUE_OPEN_LOOP_MIN     (-1000)
#define LK_TORQUE_OPEN_LOOP_MAX     (1000)

#define LK_TORQUE_CLOSED_LOOP_MIN   (-2000)
#define LK_TORQUE_CLOSED_LOOP_MAX   (2000)

// ==================== 命令 ID ====================
/**
 * @brief LK 电机命令枚举
 */
typedef enum {
    LK_CMD_GET_MULTI_TURN_ANGLE = 0x92,           ///< 读取多圈角度
    LK_CMD_GET_SINGLE_TURN_ANGLE = 0x94,          ///< 读取单圈角度
    LK_CMD_GET_STATUS_1_AND_ERROR = 0x9A,         ///< 读取电机状态1与错误标志
    LK_CMD_CLEAR_ERROR_FLAG = 0x9B,               ///< 清除错误标志
    LK_CMD_GET_STATUS_2 = 0x9C,                   ///< 读取电机状态2
    LK_CMD_GET_STATUS_3 = 0x9D,                   ///< 读取电机状态3

    LK_CMD_TORQUE_OPEN_LOOP = 0xA0,               ///< 转矩开环控制
    LK_CMD_TORQUE_CLOSED_LOOP = 0xA1,             ///< 转矩闭环控制
    LK_CMD_SPEED_CONTROL = 0xA2,                  ///< 速度控制

    LK_CMD_STOP = 0x81,                           ///< 电机停止
    LK_CMD_DISABLE = 0x80,                        ///< 电机关闭
    LK_CMD_ENABLE = 0x88                          ///< 电机运行/开启
} LK_CMD_ID;

/* ==================== 函数声明 ==================== */
/**
 * @brief 发送读取多圈角度命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void LK_GetMultiTurnAngle(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID);

/**
 * @brief 发送读取单圈角度命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void LK_GetSingleTurnAngle(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID);

/**
 * @brief 发送读取状态1与错误标志命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void LK_GetStatus1AndError(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID);

/**
 * @brief 发送清除错误标志命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void LK_ClearErrorFlag(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID);

/**
 * @brief 发送读取状态2命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void LK_GetStatus2(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID);

/**
 * @brief 发送读取状态3命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void LK_GetStatus3(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID);

/**
 * @brief 发送转矩开环控制命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 * @param torqueValue 转矩值
 */
void LK_TorqueOpenLoop(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID, int16_t torqueValue);

/**
 * @brief 发送转矩闭环控制命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 * @param torqueValue 转矩值
 */
void LK_TorqueClosedLoop(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID, int16_t torqueValue);

/**
 * @brief 发送电机速度控制命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 * @param speedControl 速度值
 */
void LK_SpeedControl(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID, int32_t speedControl);

/**
 * @brief 发送电机停止命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void LK_StopMotor(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID);

/**
 * @brief 发送电机关闭命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void LK_DisableMotor(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID);

/**
 * @brief 发送电机使能（运行）命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void LK_EnableMotor(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID);

#endif //SAFEPOWER_MOTOR_DRIVE_LK_H
