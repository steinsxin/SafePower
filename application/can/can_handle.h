//
// Created by Steins_Xin on 2025/7/29.
//

#ifndef SAFEPOWER_CAN_HANDLE_H
#define SAFEPOWER_CAN_HANDLE_H

#include "can.h"
#include "motor_config.h"

// 判断是否属于某个ID范围
#define IS_LK_LEFT_ID(id)     ((id) >= LEFT_ARM_START_ID && (id) <= LEFT_ARM_END_ID)
#define IS_LK_RIGHT_ID(id)    ((id) >= RIGHT_ARM_START_ID && (id) <= RIGHT_ARM_END_ID)
#define IS_EU_ID(id)          ((id) >= WAIST_LEG_START_ID  && (id) <= WAIST_LEG_END_ID)

/**
 * 发送CAN消息
 * @param hcan CAN总线
 * @param CAN_ID 消息ID
 * @param data 消息数据
 * @param len 数据长度
 */
void CAN_Send(CAN_HandleTypeDef *hcan, uint32_t CAN_ID, uint8_t *data, uint8_t len);

#endif //SAFEPOWER_CAN_HANDLE_H
