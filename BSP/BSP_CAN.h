//
// Created by Steins_Xin on 2025/4/1.
//

#ifndef SAFEPOWER_BSP_CAN_H
#define SAFEPOWER_BSP_CAN_H

#include "can.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

void BSP_CAN1_Init();
void BSP_CAN2_Init();

#endif //SAFEPOWER_BSP_CAN_H
