//
// Created by Steins_Xin on 2025/7/30.
//

#include "can_protocol.h"
extern MotorDevice *motor_list[MAX_MOTORS]; // 全局电机列表

/**
 * @brief 根据 CAN ID 查找对应的电机对象
 * @param can_id 接收到的 CAN 标准 ID
 * @return 指向对应 MotorDevice 的指针，若未找到则返回 NULL
 */
MotorDevice *Motor_FindByID(uint16_t can_id) {
    for (int i = 0; i < MAX_MOTORS; ++i) {
        if (motor_list[i] && motor_list[i]->can_id == can_id) {
            return motor_list[i];
        }
    }
    return NULL;
}

/**
 * @brief 根据解析出的电机消息类型调用对应的反馈处理函数
 * @param motor    指向目标电机的结构体指针
 * @param msg_type 电机消息类型（如速度控制反馈、位置控制反馈等）
 * @param data     接收到的8字节 CAN 数据内容
 */
void Handle_Motor_Feedback(MotorDevice *motor, CAN_Motor_MsgType msg_type, const uint8_t *data)
{
    if (!motor || !motor->ops) return;

    switch (msg_type) {
        case CAN_Motor_MSG_GET_ANGLE_HANDLE:
            if (motor->ops->handle_get_angle_feedback)
                motor->ops->handle_get_angle_feedback(motor, data, 8);
            break;
        case CAN_Motor_MSG_SPEED_CONTROL_HANDLE:
            if (motor->ops->handle_speed_control_feedback)
                motor->ops->handle_speed_control_feedback(motor, data, 8);
            break;
        case CAN_Motor_MSG_POSITION_CONTROL_HANDLE:
            if (motor->ops->handle_position_control_feedback)
                motor->ops->handle_position_control_feedback(motor, data, 8);
            break;
        case CAN_Motor_MSG_TORQUE_CONTROL_HANDLE:
            if (motor->ops->handle_torque_control_feedback)
                motor->ops->handle_torque_control_feedback(motor, data, 8);
            break;
        case CAN_Motor_MSG_STOP_HANDLE:
            if (motor->ops->handle_stop_feedback)
                motor->ops->handle_stop_feedback(motor, data, 8);
            break;
        case CAN_Motor_MSG_ENABLE_HANDLE:
            if (motor->ops->handle_enable_feedback)
                motor->ops->handle_enable_feedback(motor, data, 8);
            break;
        case CAN_Motor_MSG_DISENABLE_HANDLE:
            if (motor->ops->handle_disable_feedback)
                motor->ops->handle_disable_feedback(motor, data, 8);
            break;
        default:
            break;
    }
}

/**
 * @brief 处理系统类命令（如启动、停止、重启等）
 * @param sys_msg 系统命令类型
 */
void Handle_System_Command(CAN_System_MsgType sys_msg)
{
    switch (sys_msg) {
        case CAN_System_MSG_STOP:
            // 停止所有电机
            break;
        case CAN_System_MSG_START:
            // 启动所有电机
            break;
        case CAN_System_MSG_RESTART:
            // 重启系统
            break;
        default:
            break;
    }
}

/**
 * @brief 解析接收到的CAN消息，返回对应的命令类型（电机或系统）
 * @param can_id   接收到的CAN ID
 * @param data     接收到的8字节数据
 * @param msg_type 返回的电机消息类型（可为空）
 * @param sys_type 返回的系统消息类型（可为空）
 */
void Parse_CAN_Message(uint16_t can_id, uint8_t *data,
                       CAN_Motor_MsgType *msg_type,
                       CAN_System_MsgType *sys_type)
{
    // 获取命令码（data[0]）
    uint8_t cmd = data[0];

    // -------------------- 电机类 ID 解析 --------------------
    if (IS_LK_LEFT_ID(can_id) || IS_LK_RIGHT_ID(can_id)) {
        if (!msg_type) return;

        switch (cmd) {
            case LK_CMD_GET_MULTI_TURN_ANGLE:
                *msg_type = (data[1] | data[2] | data[3] | data[4] |
                             data[5] | data[6] | data[7]) == 0
                            ? CAN_Motor_MSG_GET_ANGLE
                            : CAN_Motor_MSG_GET_ANGLE_HANDLE;
                break;
            case LK_CMD_SPEED_CONTROL:
                *msg_type = (data[1] | data[2] | data[3]) == 0
                            ? CAN_Motor_MSG_SPEED_CONTROL
                            : CAN_Motor_MSG_SPEED_CONTROL_HANDLE;
                break;
            case LK_CMD_POSITION_CONTROL_MULTI:
                *msg_type = (data[1]) == 0
                            ? CAN_Motor_MSG_POSITION_CONTROL
                            : CAN_Motor_MSG_POSITION_CONTROL_HANDLE;
                break;
            case LK_CMD_POSITION_CONTROL_SINGLE:
                *msg_type = (data[6] | data[7]) == 0
                            ? CAN_Motor_MSG_POSITION_CONTROL
                            : CAN_Motor_MSG_POSITION_CONTROL_HANDLE;
                break;
            case LK_CMD_TORQUE_CLOSED_LOOP:
            case LK_CMD_TORQUE_OPEN_LOOP:
                *msg_type = (data[1] | data[2] | data[3]) == 0
                            ? CAN_Motor_MSG_TORQUE_CONTROL
                            : CAN_Motor_MSG_TORQUE_CONTROL_HANDLE;
                break;
            case LK_CMD_STOP:
                *msg_type = CAN_Motor_MSG_STOP_HANDLE;
                break;
            case LK_CMD_ENABLE:
                *msg_type = CAN_Motor_MSG_ENABLE_HANDLE;
                break;
            case LK_CMD_DISABLE:
                *msg_type = CAN_Motor_MSG_DISENABLE_HANDLE;
                break;
            default:
                *msg_type = CAN_Motor_MSG_UNKNOWN;
                break;
        }
    }
    else if (IS_EU_ID(can_id)){

    }
    // -------------------- 系统类 ID 解析 --------------------
    else if (IS_SYSTEM_ID(can_id)) {
        if (!sys_type) return;

        switch (cmd) {
            case 0x01:
                *sys_type = CAN_System_MSG_STOP;
                break;
            case 0x02:
                *sys_type = CAN_System_MSG_START;
                break;
            case 0x03:
                *sys_type = CAN_System_MSG_RESTART;
                break;
            default:
                *sys_type = CAN_System_MSG_UNKNOWN;
                break;
        }
    }
}