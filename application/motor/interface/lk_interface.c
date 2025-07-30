//
// Created by Steins_Xin on 2025/7/29.
//

#include "motor_interface.h"
#include "driver/motor_drive_lk.h"

// ======================== 电机发送处理 ========================
static void motor_lk_get_angle(MotorDevice *motor) {
    LK_GetMultiTurnAngle(motor->can_bus, motor->can_id);
}

static void motor_lk_speed_control(MotorDevice *motor, float speed) {
    LK_SpeedControl(motor->can_bus, motor->can_id, speed);
}

static void motor_lk_position_control(MotorDevice *motor, float angleControl, uint16_t maxSpeed) {
    LK_PositionControlMulti(motor->can_bus, motor->can_id, angleControl, maxSpeed);
}

static void motor_lk_torque_control(MotorDevice *motor, int16_t torque) {
    LK_TorqueOpenLoop(motor->can_bus, motor->can_id, torque);
}

static void motor_lk_stop(MotorDevice *motor) {
    LK_StopMotor(motor->can_bus, motor->can_id);
}

static void motor_lk_enable(MotorDevice *motor) {
    LK_EnableMotor(motor->can_bus, motor->can_id);
}

static void motor_lk_disable(MotorDevice *motor) {
    LK_DisableMotor(motor->can_bus, motor->can_id);
}

// ======================== 电机回复处理 ========================
static void motor_lk_get_angle_feedback(MotorDevice *motor, const uint8_t *data, uint8_t len) {
    LK_HandleMultiTurnAngleFeedback(motor, data, len);
}

static void motor_lk_speed_control_feedback(MotorDevice *motor, const uint8_t *data, uint8_t len) {
    LK_HandleSpeedControlFeedback(motor, data, len);
}

static void motor_lk_position_control_feedback(MotorDevice *motor, const uint8_t *data, uint8_t len) {
    LK_HandlePositionControlMultiFeedback(motor, data, len);
}

static void motor_lk_torque_control_feedback(MotorDevice *motor, const uint8_t *data, uint8_t len) {
    LK_HandleTorqueOpenLoopFeedback(motor, data, len);
}

static void motor_lk_stop_feedback(MotorDevice *motor, const uint8_t *data, uint8_t len) {
    LK_HandleStopMotorFeedback(motor, data, len);
}

static void motor_lk_enable_feedback(MotorDevice *motor, const uint8_t *data, uint8_t len) {
    LK_HandleEnableMotorFeedback(motor, data, len);
}

static void motor_lk_disable_feedback(MotorDevice *motor, const uint8_t *data, uint8_t len) {
    LK_HandleDisableMotorFeedback(motor, data, len);
}


// 定义 LK 电机的操作表
const MotorOps LKMotorOps = {
        .get_angle = motor_lk_get_angle,
        .speed_control = motor_lk_speed_control,
        .position_control = motor_lk_position_control,
        .torque_control = motor_lk_torque_control,
        .stop = motor_lk_stop,
        .enable = motor_lk_enable,
        .disable = motor_lk_disable,

        .handle_get_angle_feedback = motor_lk_get_angle_feedback,
        .handle_speed_control_feedback = motor_lk_speed_control_feedback,
        .handle_position_control_feedback = motor_lk_position_control_feedback,
        .handle_torque_control_feedback = motor_lk_torque_control_feedback,
        .handle_stop_feedback = motor_lk_stop_feedback,
        .handle_enable_feedback = motor_lk_enable_feedback,
        .handle_disable_feedback = motor_lk_disable_feedback
};