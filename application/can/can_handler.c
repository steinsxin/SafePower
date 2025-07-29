//
// Created by Steins_Xin on 2025/7/29.
//

#include "can_handle.h"

/**
 * 发送CAN消息
 * @param hcan CAN总线
 * @param CAN_ID 消息ID
 * @param data 消息数据
 * @param len 数据长度
 */
void CAN_Send(CAN_HandleTypeDef *hcan, uint32_t CAN_ID, uint8_t *data, uint8_t len) {
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