//
// Created by Steins_Xin on 2025/7/29.
//

#include "motor_drive_eu.h"

/* ==================== 函数声明 ==================== */
extern void CAN_Send(CAN_HandleTypeDef *hcan, uint32_t CAN_ID, uint8_t *data, uint8_t len);

/**
 * @brief 发送读取多圈角度命令
 * @param CAN_Bus CAN总线句柄
 * @param CAN_ID 电机ID
 */
void EU_GetMultiTurnAngle(CAN_HandleTypeDef *CAN_Bus, uint32_t CAN_ID){

}

/**
 * @brief 发送读取单圈角度命令
 * @param CAN_Bus CAN总线句柄
 * @param CAN_ID 电机ID
 */
void EU_GetSingleTurnAngle(CAN_HandleTypeDef *CAN_Bus, uint32_t CAN_ID){

}

/**
 * @brief 发送读取状态1与错误标志命令
 * @param CAN_Bus CAN总线句柄
 * @param CAN_ID 电机ID
 */
void EU_GetStatus1AndError(CAN_HandleTypeDef *CAN_Bus, uint32_t CAN_ID){

}

/**
 * @brief 发送清除错误标志命令
 * @param CAN_Bus CAN总线句柄
 * @param CAN_ID 电机ID
 */
void EU_ClearErrorFlag(CAN_HandleTypeDef *CAN_Bus, uint32_t CAN_ID){

}

/**
 * @brief 发送转矩开环控制命令
 * @param CAN_Bus CAN总线句柄
 * @param CAN_ID 电机ID
 * @param torqueValue 转矩值
 */
void EU_TorqueOpenLoop(CAN_HandleTypeDef *CAN_Bus, uint32_t CAN_ID, int16_t torqueValue){

}

/**
 * @brief 发送转矩闭环控制命令
 * @param CAN_Bus CAN总线句柄
 * @param CAN_ID 电机ID
 * @param torqueValue 转矩值
 */
void EU_TorqueClosedLoop(CAN_HandleTypeDef *CAN_Bus, uint32_t CAN_ID, int16_t torqueValue){

}

/**
 * @brief 发送电机速度控制命令
 * @param CAN_Bus CAN总线句柄
 * @param CAN_ID 电机ID
 * @param speedControl 速度值
 */
void EU_SpeedControl(CAN_HandleTypeDef *CAN_Bus, uint32_t CAN_ID, int32_t speedControl){

}

/**
 * @brief 发送电机停止命令
 * @param CAN_Bus CAN总线句柄
 * @param CAN_ID 电机ID
 */
void EU_StopMotor(CAN_HandleTypeDef *CAN_Bus, uint32_t CAN_ID){

}

/**
 * @brief 发送电机关闭命令
 * @param CAN_Bus CAN总线句柄
 * @param CAN_ID 电机ID
 */
void EU_DisableMotor(CAN_HandleTypeDef *CAN_Bus, uint32_t CAN_ID){

}

/**
 * @brief 发送电机使能（运行）命令
 * @param CAN_Bus CAN总线句柄
 * @param CAN_ID 电机ID
 */
void EU_EnableMotor(CAN_HandleTypeDef *CAN_Bus, uint32_t CAN_ID){

}