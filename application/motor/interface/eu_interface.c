//
// Created by Steins_Xin on 2025/7/29.
//

#include "motor_interface.h"
#include "driver/motor_drive_eu.h"

//static void motor_eu_get_angle(MotorDevice *motor) {
//    EU_GetMultiTurnAngle(motor->can_bus, motor->can_id);
//}
//
//static void motor_eu_speed_control(MotorDevice *motor, int32_t speed) {
//    EU_SpeedControl(motor->can_bus, motor->can_id, speed);
//}
//
//static void motor_eu_torque_control(MotorDevice *motor, int16_t torque) {
//    EU_TorqueOpenLoop(motor->can_bus, motor->can_id, torque);
//}
//
//static void motor_eu_stop(MotorDevice *motor) {
//    EU_StopMotor(motor->can_bus, motor->can_id);
//}
//
//static void motor_eu_enable(MotorDevice *motor) {
//    EU_EnableMotor(motor->can_bus, motor->can_id);
//}
//
//static void motor_eu_disable(MotorDevice *motor) {
//    EU_DisableMotor(motor->can_bus, motor->can_id);
//}
//
//// 定义 EU 电机的操作表
//const MotorOps EUMotorOps = {
//        .get_angle = motor_eu_get_angle,
//        .speed_control = motor_eu_speed_control,
//        .torque_control = motor_eu_torque_control,
//        .stop = motor_eu_stop,
//        .enable = motor_eu_enable,
//        .disable = motor_eu_disable
//};