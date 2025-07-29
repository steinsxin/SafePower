//
// Created by Steins_Xin on 2025/4/2.
//

#include "MotorDrive.h"

/* ==================== 静态函数声明 ==================== */
static void CAN_Send(CAN_HandleTypeDef *hcan, uint32_t CAN_ID, uint8_t *data, uint8_t len);

/* ==================== 函数实现 ==================== */

/**
 * 发送获取电机角度命令
 * @param CAN_Bus CAN总线
 * @param CAN_ID 电机ID
 */
void LK_Angle_CMD(CAN_HandleTypeDef *CAN_Bus, uint32_t CAN_ID) {
    uint8_t LK_Motor_Angle[8] = {LK_CMD_GET_ANGLE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};   // 获取电机角度命令

    // 发送电机角度获取命令
    CAN_Send(CAN_Bus, CAN_ID, LK_Motor_Angle, 8);
}

/**
 * 发送电机停止命令
 * @param CAN_Bus CAN总线
 * @param CAN_ID 电机ID
 */
void LK_Stop_CMD(CAN_HandleTypeDef *CAN_Bus, uint32_t CAN_ID) {
    uint8_t LK_Motor_Stop[8] = {LK_CMD_STOP, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};    // 电机停止命令

    // 发送电机停止命令
    CAN_Send(CAN_Bus, CAN_ID, LK_Motor_Stop, 8);
}

/**
 * 发送电机速度控制命令
 * @param CAN_Bus CAN总线
 * @param CAN_ID 电机ID
 * @param speedControl 速度控制值
 */
void LK_Speed_CMD(CAN_HandleTypeDef *CAN_Bus, uint32_t CAN_ID, int32_t speedControl) {
    uint8_t LK_Motor_Speed[8] = {LK_CMD_SPEED_CONTROL, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};   // 电机速度命令

    // 将速度控制值转换为字节数组
    LK_Motor_Speed[4] = *(uint8_t *)(&speedControl);      // 最低字节 (LSB)
    LK_Motor_Speed[5] = *((uint8_t *)(&speedControl) + 1);  // 次低字节
    LK_Motor_Speed[6] = *((uint8_t *)(&speedControl) + 2);  // 次高字节
    LK_Motor_Speed[7] = *((uint8_t *)(&speedControl) + 3);  // 最高字节 (MSB)

    // 发送电机速度控制命令
    CAN_Send(CAN_Bus, CAN_ID, LK_Motor_Speed, 8);
}

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

/* ==================== 静态函数实现 ==================== */

/**
 * 发送CAN消息
 * @param hcan CAN总线
 * @param CAN_ID 消息ID
 * @param data 消息数据
 * @param len 数据长度
 */
static void CAN_Send(CAN_HandleTypeDef *hcan, uint32_t CAN_ID, uint8_t *data, uint8_t len) {
    uint32_t send_mail_box;
    CAN_TxHeaderTypeDef tx_message = {
            .StdId = CAN_ID,
            .IDE = CAN_ID_STD,
            .RTR = CAN_RTR_DATA,
            .DLC = len,
    };

    // 发送CAN消息
    HAL_CAN_AddTxMessage(hcan, &tx_message, data, &send_mail_box);
}