//
// Created by Steins_Xin on 2025/7/29.
//

#ifndef SAFEPOWER_CAN_PROTOCOL_H
#define SAFEPOWER_CAN_PROTOCOL_H

// CAN命令解析
#include "system_config.h"
#include "motor_interface.h"
#include "driver/motor_drive_lk.h"
#include "driver/motor_drive_eu.h"

// 判断是否属于某个ID范围
#define IS_SYSTEM_ID(id)      ((id) == SYSTEM_ID)
#define IS_LK_LEFT_ID(id)     ((id) >= LEFT_ARM_START_ID && (id) <= LEFT_ARM_END_ID)
#define IS_LK_RIGHT_ID(id)    ((id) >= RIGHT_ARM_START_ID && (id) <= RIGHT_ARM_END_ID)
#define IS_EU_ID(id)          ((id) >= WAIST_LEG_START_ID  && (id) <= WAIST_LEG_END_ID)


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

/**
 * @brief 根据 CAN ID 查找对应的电机对象
 * @param can_id 接收到的 CAN 标准 ID
 * @return 指向对应 MotorDevice 的指针，若未找到则返回 NULL
 */
MotorDevice *Motor_FindByID(uint16_t can_id);

/**
 * @brief 解析接收到的CAN消息，返回对应的命令类型（电机或系统）
 * @param can_id   接收到的CAN ID
 * @param data     接收到的8字节数据
 * @param msg_type 返回的电机消息类型（可为空）
 * @param sys_type 返回的系统消息类型（可为空）
 */
void Parse_CAN_Message(uint16_t can_id, uint8_t *data, CAN_Motor_MsgType *msg_type, CAN_System_MsgType *sys_type);

/**
 * @brief 根据解析出的电机消息类型调用对应的反馈处理函数
 * @param motor    指向目标电机的结构体指针
 * @param msg_type 电机消息类型（如速度控制反馈、位置控制反馈等）
 * @param data     接收到的8字节 CAN 数据内容
 */
void Handle_Motor_Feedback(MotorDevice *motor, CAN_Motor_MsgType msg_type, const uint8_t *data);

/**
 * @brief 处理系统类命令（如启动、停止、重启等）
 * @param sys_msg 系统命令类型
 */
void Handle_System_Command(CAN_System_MsgType sys_msg);

#endif //SAFEPOWER_CAN_PROTOCOL_H
