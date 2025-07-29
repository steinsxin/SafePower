//
// Created by Steins_Xin on 2025/7/29.
//

#include "motor_drive_eu.h"

/* ==================== 函数声明 ==================== */
extern void CAN_Send(CAN_HandleTypeDef *hcan, uint32_t CAN_ID, uint8_t *data, uint8_t len);

/**
 * 发送获取电机角度命令
 * @param CAN_Bus CAN总线
 * @param CAN_ID 电机ID
 */
void EU_Get_Angle_CMD(CAN_HandleTypeDef *CAN_Bus, uint32_t CAN_ID){
    uint8_t EU_Motor_Get_Angle[6] = {EU_CMD_GET_ANGLE, 0x07, 0x00, 0x00, 0x00, 0x00};   // 电机角度命令

    CAN_Send(CAN_Bus,CAN_ID,EU_Motor_Get_Angle,6);
}

/**
 * 发送电机速度控制命令
 * @param CAN_Bus CAN总线
 * @param CAN_ID 电机ID
 * @param speedControl 速度控制值
 */
void EU_Speed_CMD(CAN_HandleTypeDef *CAN_Bus, uint32_t CAN_ID, int32_t speedControl){
    uint8_t EU_Motor_Speed[6] = {EU_CMD_SPEED_CONTROL, 0x09, 0x00, 0x00, 0x00, 0x00};   // 电机速度命令

    // 发送电机速度停止命令
    CAN_Send(CAN_Bus,CAN_ID,EU_Motor_Speed,6);
}
