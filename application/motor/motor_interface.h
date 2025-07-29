//
// Created by Steins_Xin on 2025/7/29.
//

#ifndef SAFEPOWER_MOTOR_INTERFACE_H
#define SAFEPOWER_MOTOR_INTERFACE_H

// 使用示例
//MotorDevice left_shoulder = {
//        .type = MOTOR_TYPE_LK,
//        .id = 0x145,
//        .can_bus = &hcan1
//};
//
//MotorDevice waist_motor = {
//        .type = MOTOR_TYPE_EU,
//        .id = 0x15,
//        .can_bus = &hcan2
//};
//
//// 控制逻辑一致，不关心底层型号
//Motor_GetMultiTurnAngle(&left_shoulder);
//Motor_SpeedControl(&waist_motor, 1200);
//Motor_Stop(&left_shoulder);

#include "can.h"
#include <stdint.h>

typedef enum {
    MOTOR_LK,
    MOTOR_EU,
    // 后续可以扩展：MOTOR_XXX
} MotorType;

// 前向声明
struct MotorDevice;

// 定义函数指针接口
typedef struct {
    void (*get_angle)(struct MotorDevice *motor);
    void (*speed_control)(struct MotorDevice *motor, int32_t speed);
    void (*torque_control)(struct MotorDevice *motor, int16_t torque);
    void (*stop)(struct MotorDevice *motor);
    void (*enable)(struct MotorDevice *motor);
    void (*disable)(struct MotorDevice *motor);
} MotorOps;

// 公共接口宏包装
#define Motor_GetMultiTurnAngle(m)     ((m)->ops->get_angle(m))
#define Motor_SpeedControl(m, s)       ((m)->ops->speed_control(m, s))
#define Motor_TorqueOpenLoop(m, t)     ((m)->ops->torque_control(m, t))
#define Motor_Stop(m)                  ((m)->ops->stop(m))
#define Motor_Enable(m)                ((m)->ops->enable(m))
#define Motor_Disable(m)               ((m)->ops->disable(m))

// 电机设备结构体
typedef struct MotorDevice {
    const char *name;         // 可选：电机角色，如 "left_shoulder"
    MotorType type;
    uint32_t id;
    CAN_HandleTypeDef *can_bus;
    const MotorOps *ops;      // 指向该电机类型的操作函数集
} MotorDevice;

#endif //SAFEPOWER_MOTOR_INTERFACE_H
