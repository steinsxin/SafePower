//
// Created by Steins_Xin on 2025/7/29.
//

#include "motor_drive_lk.h"

/**
 * @brief 发送读取多圈角度命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void LK_GetMultiTurnAngle(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID){
    uint8_t data[8];
    memset(data, 0, sizeof(data));  // 全部初始化为 0

    data[0] = LK_CMD_GET_MULTI_TURN_ANGLE;  // 设置命令字节

    CAN_Send(CAN_BUS, CAN_ID, data, 8);     // 发送完整8字节
}

/**
 * @brief 发送读取单圈角度命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void LK_GetSingleTurnAngle(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID){
    uint8_t data[8];
    memset(data, 0, sizeof(data));  // 全部初始化为 0

    data[0] = LK_CMD_GET_SINGLE_TURN_ANGLE;  // 设置命令字节

    CAN_Send(CAN_BUS, CAN_ID, data, 8);     // 发送完整8字节
}

/**
 * @brief 发送读取状态1与错误标志命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void LK_GetStatus1AndError(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID){
    uint8_t data[8];
    memset(data, 0, sizeof(data));  // 全部初始化为 0

    data[0] = LK_CMD_GET_STATUS_1_AND_ERROR;  // 设置命令字节

    CAN_Send(CAN_BUS, CAN_ID, data, 8);     // 发送完整8字节
}

/**
 * @brief 发送清除错误标志命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void LK_ClearErrorFlag(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID){
    uint8_t data[8];
    memset(data, 0, sizeof(data));  // 全部初始化为 0

    data[0] = LK_CMD_CLEAR_ERROR_FLAG;  // 设置命令字节

    CAN_Send(CAN_BUS, CAN_ID, data, 8);     // 发送完整8字节
}

/**
 * @brief 发送读取状态2命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void LK_GetStatus2(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID){
    uint8_t data[8];
    memset(data, 0, sizeof(data));  // 全部初始化为 0

    data[0] = LK_CMD_GET_STATUS_2;  // 设置命令字节

    CAN_Send(CAN_BUS, CAN_ID, data, 8);     // 发送完整8字节
}

/**
 * @brief 发送读取状态3命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void LK_GetStatus3(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID){
    uint8_t data[8];
    memset(data, 0, sizeof(data));  // 全部初始化为 0

    data[0] = LK_CMD_GET_STATUS_3;  // 设置命令字节

    CAN_Send(CAN_BUS, CAN_ID, data, 8);     // 发送完整8字节
}

/**
 * @brief 发送转矩开环控制命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 * @param torqueValue 转矩值
 */
void LK_TorqueOpenLoop(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID, int16_t torqueValue){
    // 限幅
    if (torqueValue > LK_TORQUE_OPEN_LOOP_MAX) {
        torqueValue = LK_TORQUE_OPEN_LOOP_MAX;
    } else if (torqueValue < LK_TORQUE_OPEN_LOOP_MIN) {
        torqueValue = LK_TORQUE_OPEN_LOOP_MIN;
    }

    uint8_t data[8];
    memset(data, 0, sizeof(data));  // 初始化为0

    data[0] = LK_CMD_TORQUE_OPEN_LOOP;
    data[4] = (uint8_t)(torqueValue & 0xFF);         // 低字节
    data[5] = (uint8_t)((torqueValue >> 8) & 0xFF);  // 高字节

    CAN_Send(CAN_BUS, CAN_ID, data, 8);
}

/**
 * @brief 发送转矩闭环控制命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 * @param torqueValue 转矩值
 */
void LK_TorqueClosedLoop(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID, int16_t torqueValue){
    // 限幅
    if (torqueValue > LK_TORQUE_CLOSED_LOOP_MAX) {
        torqueValue = LK_TORQUE_CLOSED_LOOP_MAX;
    } else if (torqueValue < LK_TORQUE_CLOSED_LOOP_MIN) {
        torqueValue = LK_TORQUE_CLOSED_LOOP_MIN;
    }

    uint8_t data[8] = {0};
    data[0] = LK_CMD_TORQUE_CLOSED_LOOP;

    data[4] = (uint8_t)(torqueValue & 0xFF);         // 低字节
    data[5] = (uint8_t)((torqueValue >> 8) & 0xFF);  // 高字节

    CAN_Send(CAN_BUS, CAN_ID, data, 8);
}

/**
 * @brief 发送电机速度控制命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 * @param speedControl 速度值
 */
void LK_SpeedControl(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID, int32_t speedControl){
    uint8_t data[8] = {0};

    data[0] = LK_CMD_SPEED_CONTROL;

    // 拆分 speedControl 成 4 个字节，低位在前（小端序）
    data[4] = (uint8_t)(speedControl & 0xFF);
    data[5] = (uint8_t)((speedControl >> 8) & 0xFF);
    data[6] = (uint8_t)((speedControl >> 16) & 0xFF);
    data[7] = (uint8_t)((speedControl >> 24) & 0xFF);

    CAN_Send(CAN_BUS, CAN_ID, data, 8);
}

/**
 * @brief 发送电机停止命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void LK_StopMotor(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID){
    uint8_t data[8];
    memset(data, 0, sizeof(data));  // 全部初始化为 0

    data[0] = LK_CMD_STOP;  // 设置命令字节

    CAN_Send(CAN_BUS, CAN_ID, data, 8);     // 发送完整8字节
}

/**
 * @brief 发送电机关闭命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void LK_DisableMotor(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID){
    uint8_t data[8];
    memset(data, 0, sizeof(data));  // 全部初始化为 0

    data[0] = LK_CMD_DISABLE;  // 设置命令字节

    CAN_Send(CAN_BUS, CAN_ID, data, 8);     // 发送完整8字节
}

/**
 * @brief 发送电机使能（运行）命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void LK_EnableMotor(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID){
    uint8_t data[8];
    memset(data, 0, sizeof(data));  // 全部初始化为 0

    data[0] = LK_CMD_ENABLE;  // 设置命令字节

    CAN_Send(CAN_BUS, CAN_ID, data, 8);     // 发送完整8字节
}