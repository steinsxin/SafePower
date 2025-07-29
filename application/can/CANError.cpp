//
// Created by Steins_Xin on 2025/7/29.
//

#include "CANError.h"

/**
 * @brief CAN错误处理
 * @param hcan CAN句柄指针
 */
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan) {
    if (hcan->ErrorCode & HAL_CAN_ERROR_BOF) {
        // 已进入 Bus Off 状态
        // 建议上报错误 + 尝试重启
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
