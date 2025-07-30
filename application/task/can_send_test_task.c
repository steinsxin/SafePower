//
// Created by Steins_Xin on 2025/7/30.
//

#include "can_send_test_task.h"

extern const MotorOps LKMotorOps;

/**
 * @brief CAN发送测试任务
 * @param argument 任务参数
 */
void can_send_test_task(void const * argument)
{
    LED_ON(LED3);
    static MotorDevice left_shoulder_motor = {
            .name = "left_shoulder",
            .type = MOTOR_LK,
            .can_id = 0x145,
            .can_bus = &hcan1,
            .ops = &LKMotorOps,
            .context = NULL,
    };
    Motor_Enable(&left_shoulder_motor);
    while (1) {
//        Motor_SpeedControl(&left_shoulder_motor, 60);
        Motor_GetMultiTurnAngle(&left_shoulder_motor);
        vTaskDelay(50);  /* 5ms周期监控 */
        LED_TOGGLE(LED3);
    }
}