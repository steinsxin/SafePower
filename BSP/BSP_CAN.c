//
// Created by Steins_Xin on 2025/4/1.
//
#include "BSP_CAN.h"

/**
 * CAN1 初始化
 */
void BSP_CAN1_Init(){

    CAN_FilterTypeDef CAN_Filter;
    CAN_Filter.FilterBank = 0;                          // 使用过滤器组0
    CAN_Filter.FilterMode = CAN_FILTERMODE_IDMASK;      // 掩码模式
    CAN_Filter.FilterScale = CAN_FILTERSCALE_32BIT;     // 32位宽
    CAN_Filter.FilterIdHigh = 0x0000;                   // ID高16位（全0表示不匹配特定ID）
    CAN_Filter.FilterIdLow = 0x0000;                    // ID低16位
    CAN_Filter.FilterMaskIdHigh = 0x0000;               // 掩码高16位（全0表示接收所有ID）
    CAN_Filter.FilterMaskIdLow = 0x0000;                // 掩码低16位
    CAN_Filter.FilterFIFOAssignment = CAN_RX_FIFO0;     // 数据存入FIFO0
    CAN_Filter.FilterActivation = ENABLE;               // 启用过滤器
    HAL_CAN_ConfigFilter(&hcan1, &CAN_Filter);

    // 启动CAN并启用接收中断
    HAL_CAN_Start(&hcan1);
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);  // 启用FIFO0接收中断
}

/**
 * CAN2 初始化
 */
void BSP_CAN2_Init(){

    CAN_FilterTypeDef CAN_Filter;
    CAN_Filter.FilterBank = 14;                         // 使用过滤器组0
    CAN_Filter.SlaveStartFilterBank = 14;               // CAN2的起始过滤器组
    CAN_Filter.FilterMode = CAN_FILTERMODE_IDMASK;      // 掩码模式
    CAN_Filter.FilterScale = CAN_FILTERSCALE_32BIT;     // 32位宽
    CAN_Filter.FilterIdHigh = 0x0000;                   // ID高16位（全0表示不匹配特定ID）
    CAN_Filter.FilterIdLow = 0x0000;                    // ID低16位
    CAN_Filter.FilterMaskIdHigh = 0x0000;               // 掩码高16位（全0表示接收所有ID）
    CAN_Filter.FilterMaskIdLow = 0x0000;                // 掩码低16位
    CAN_Filter.FilterFIFOAssignment = CAN_RX_FIFO0;     // 数据存入FIFO0
    CAN_Filter.FilterActivation = ENABLE;               // 启用过滤器
    HAL_CAN_ConfigFilter(&hcan2, &CAN_Filter);

    // 启动CAN并启用接收中断
    HAL_CAN_Start(&hcan2);
    HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);  // 启用FIFO0接收中断
}