//
// Created by Steins_Xin on 2025/7/29.
//

#include "can_handle.h"

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
        // 进行命令分离
        uint16_t can_id = header.StdId;
        if (IS_LK_LEFT_ID(can_id)) {
            // LK 命令解析
        }
        else if (IS_LK_RIGHT_ID(can_id)) {
            // LK命令解析
        }
        else if (IS_EU_ID(can_id)) {
            // EU命令解析
        }
        else {
            // 未知 ID，可记录或忽略
        }

    }
}