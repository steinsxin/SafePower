//
// Created by Steins_Xin on 2025/7/29.
//

#include "can_error.h"

/**
 * @brief CAN错误处理
 * @param hcan CAN句柄指针
 */
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan) {
    if (hcan->ErrorCode & HAL_CAN_ERROR_BOF) {
        // 读取错误计数器
        uint8_t tec = (hcan->Instance->ESR >> 16) & 0xFF;
        uint8_t rec = (hcan->Instance->ESR >> 24) & 0xFF;

//        printf("CAN Bus-Off! TEC=%u, REC=%u\r\n", tec, rec);

        // 尝试重启
        HAL_CAN_Stop(hcan);
        HAL_CAN_Start(hcan);
        HAL_CAN_ActivateNotification(hcan,
             CAN_IT_RX_FIFO0_MSG_PENDING |
             CAN_IT_RX_FIFO1_MSG_PENDING |
             CAN_IT_TX_MAILBOX_EMPTY |
             CAN_IT_BUSOFF |
             CAN_IT_ERROR_WARNING |
             CAN_IT_ERROR_PASSIVE);
    }
}