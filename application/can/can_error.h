//
// Created by Steins_Xin on 2025/7/29.
//

#ifndef SAFEPOWER_CAN_ERROR_H
#define SAFEPOWER_CAN_ERROR_H

#include "can.h"
#include "stdint.h"

/**
 * @brief CAN错误管理结构体
 */
typedef struct {
    uint8_t tec;  ///< Transmit Error Counter
    uint8_t rec;  ///< Receive Error Counter
    uint8_t is_bus_off;  ///< 标记当前是否处于Bus-Off状态
} CANErrorManager;

/**
 * @brief 初始化CAN错误管理器
 * @param mgr 指向CANErrorManager实例的指针
 */
void CAN_ErrorManager_Init(CANErrorManager *mgr);

/**
 * @brief CAN错误回调函数，在 HAL_CAN_ErrorCallback 中调用
 * @param hcan CAN句柄指针
 * @param mgr CAN错误管理器指针
 */
void CAN_HandleError(CAN_HandleTypeDef *hcan, CANErrorManager *mgr);

/**
 * @brief 获取最新的错误计数
 * @param mgr CAN错误管理器指针
 * @param tec 返回的TEC指针
 * @param rec 返回的REC指针
 */
void CAN_GetErrorCounters(CANErrorManager *mgr, uint8_t *tec, uint8_t *rec);

/**
 * @brief 是否处于Bus-Off状态
 * @param mgr CAN错误管理器指针
 * @return 1: 是，0: 否
 */
uint8_t CAN_IsBusOff(CANErrorManager *mgr);

#endif //SAFEPOWER_CAN_ERROR_H
