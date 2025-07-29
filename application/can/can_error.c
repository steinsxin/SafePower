//
// Created by Steins_Xin on 2025/7/29.
//

#include "can_error.h"

CANErrorManager can_err_mgr;

/**
 * @brief 初始化CAN错误管理器
 */
void CAN_ErrorManager_Init(CANErrorManager *mgr) {
    mgr->tec = 0;
    mgr->rec = 0;
    mgr->is_bus_off = 0;
}

/**
 * @brief 处理CAN错误，包括Bus-Off检测和恢复
 */
void CAN_HandleError(CAN_HandleTypeDef *hcan, CANErrorManager *mgr) {
    // 检查是否是 Bus-Off 错误
    if (hcan->ErrorCode & HAL_CAN_ERROR_BOF) {
        mgr->is_bus_off = 1;

        // 读取 TEC 和 REC 错误计数器
        mgr->tec = (hcan->Instance->ESR >> 16) & 0xFF;
        mgr->rec = (hcan->Instance->ESR >> 24) & 0xFF;

        // 可选：打印或记录错误状态
        // printf("CAN Bus-Off! TEC=%u, REC=%u\r\n", mgr->tec, mgr->rec);

        // 执行CAN恢复流程
        HAL_CAN_Stop(hcan);
        HAL_CAN_Start(hcan);
        HAL_CAN_ActivateNotification(hcan,
                                     CAN_IT_RX_FIFO0_MSG_PENDING |
                                     CAN_IT_RX_FIFO1_MSG_PENDING |
                                     CAN_IT_TX_MAILBOX_EMPTY |
                                     CAN_IT_BUSOFF |
                                     CAN_IT_ERROR_WARNING |
                                     CAN_IT_ERROR_PASSIVE);

        // 重置标志位（可选，表示重启成功）
        mgr->is_bus_off = 0;
    }
}

/**
 * @brief 获取当前TEC和REC值
 */
void CAN_GetErrorCounters(CANErrorManager *mgr, uint8_t *tec, uint8_t *rec) {
    if (tec) *tec = mgr->tec;
    if (rec) *rec = mgr->rec;
}

/**
 * @brief 判断当前是否处于Bus-Off状态
 */
uint8_t CAN_IsBusOff(CANErrorManager *mgr) {
    return mgr->is_bus_off;
}

/**
 * @brief CAN错误处理
 * @param hcan CAN句柄指针
 */
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan) {
    if (hcan->ErrorCode & HAL_CAN_ERROR_BOF) {
        CAN_HandleError(hcan, &can_err_mgr);
    }
}