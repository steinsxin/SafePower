//
// Created by Steins_Xin on 2025/7/30.
//

#include "can_protocol.h"

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
    // 默认设为未知
    if (msg_type) *msg_type = CAN_Motor_MSG_UNKNOWN;
    if (sys_type) *sys_type = CAN_System_MSG_UNKNOWN;

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