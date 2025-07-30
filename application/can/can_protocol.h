//
// Created by Steins_Xin on 2025/7/29.
//

#ifndef SAFEPOWER_CAN_PROTOCOL_H
#define SAFEPOWER_CAN_PROTOCOL_H

// CAN命令解析
#include "driver/motor_drive_lk.h"
#include "driver/motor_drive_eu.h"

/**
 * @brief 电机 CAN 消息类型枚举（解析分类结果）
 */
typedef enum {
    CAN_Motor_MSG_UNKNOWN,                   ///< 未知消息类型

    CAN_Motor_MSG_GET_ANGLE,                 ///< 获取角度命令
    CAN_Motor_MSG_SPEED_CONTROL,             ///< 速度控制命令
    CAN_Motor_MSG_POSITION_CONTROL,          ///< 位置控制命令
    CAN_Motor_MSG_TORQUE_CONTROL,            ///< 力矩控制命令
    CAN_Motor_MSG_STOP,                      ///< 电机停止命令
    CAN_Motor_MSG_ENABLE,                    ///< 电机使能命令
    CAN_Motor_MSG_DISENABLE,                 ///< 电机失能命令

    CAN_Motor_MSG_GET_ANGLE_HANDLE,          ///< 获取角度命令反馈数据
    CAN_Motor_MSG_SPEED_CONTROL_HANDLE,      ///< 速度控制命令反馈数据
    CAN_Motor_MSG_POSITION_CONTROL_HANDLE,   ///< 位置控制命令反馈数据
    CAN_Motor_MSG_TORQUE_CONTROL_HANDLE,     ///< 力矩控制命令反馈数据
    CAN_Motor_MSG_STOP_HANDLE,               ///< 电机停止命令反馈数据
    CAN_Motor_MSG_ENABLE_HANDLE,             ///< 电机使能命令反馈数据
    CAN_Motor_MSG_DISENABLE_HANDLE,          ///< 电机失能命令反馈数据
} CAN_Motor_MsgType;

/**
 * @brief 系统 CAN 消息类型枚举（解析分类结果）
 */
typedef enum {
    CAN_System_MSG_UNKNOWN,               ///< 未知消息类型
    CAN_System_MSG_STOP,                  ///< 系统停止命令
    CAN_System_MSG_START,                 ///< 系统启动命令
    CAN_System_MSG_RESTART,               ///< 系统重启命令
} CAN_System_MsgType;

#endif //SAFEPOWER_CAN_PROTOCOL_H
