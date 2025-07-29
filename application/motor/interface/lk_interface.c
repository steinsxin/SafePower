//
// Created by Steins_Xin on 2025/7/29.
//

#include "motor_interface.h"
#include "driver/motor_drive_lk.h"

/**
    MotorDevice left_shoulder = {
        .type = MOTOR_TYPE_LK,
        .id = 0x145,
        .can_bus = &hcan1,
        .ops = &LKMotorOps
    };

    // 调用示例
    Motor_SpeedControl(&left_shoulder, 1000);
*/

static void motor_lk_get_angle(MotorDevice *motor) {
    LK_GetMultiTurnAngle(motor->can_bus, motor->id);
}

static void motor_lk_speed_control(MotorDevice *motor, int32_t speed) {
    LK_SpeedControl(motor->can_bus, motor->id, speed);
}

static void motor_lk_torque_control(MotorDevice *motor, int16_t torque) {
    LK_TorqueOpenLoop(motor->can_bus, motor->id, torque);
}

static void motor_lk_stop(MotorDevice *motor) {
    LK_StopMotor(motor->can_bus, motor->id);
}

static void motor_lk_enable(MotorDevice *motor) {
    LK_EnableMotor(motor->can_bus, motor->id);
}

static void motor_lk_disable(MotorDevice *motor) {
    LK_DisableMotor(motor->can_bus, motor->id);
}

// 定义 LK 电机的操作表
const MotorOps LKMotorOps = {
        .get_angle = motor_lk_get_angle,
        .speed_control = motor_lk_speed_control,
        .torque_control = motor_lk_torque_control,
        .stop = motor_lk_stop,
        .enable = motor_lk_enable,
        .disable = motor_lk_disable
};