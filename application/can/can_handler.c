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

/**
 * @brief CAN接收中断回调函数
 * @param hcan CAN句柄指针
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    uint8_t data[8];
    CAN_RxHeaderTypeDef header;

    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &header, data) == HAL_OK) {
        uint16_t can_id = header.StdId;

        CAN_Motor_MsgType motor_msg;
        CAN_System_MsgType sys_msg;

        Parse_CAN_Message(can_id, data, &motor_msg, &sys_msg);

        // 进行后续处理
    }
}