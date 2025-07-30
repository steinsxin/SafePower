//
// Created by Steins_Xin on 2025/7/29.
//

#ifndef SAFEPOWER_MOTOR_INTERFACE_H
#define SAFEPOWER_MOTOR_INTERFACE_H

#include "can.h"
#include "motor_config.h"
#include <stdint.h>

/**
    使用示例
    MotorDevice left_shoulder_motor = {
        .name = "left_shoulder",
        .type = MOTOR_LK,
        .can_id = 0x145,
        .can_bus = &hcan1,
        .ops = &lk_motor_ops,
        .context = NULL,
    };
    Motor_SpeedControl(&left_shoulder_motor, 100);
*/

#define Motor_GetMultiTurnAngle(m)     ((m)->ops->get_angle(m))
#define Motor_SpeedControl(m, s)       ((m)->ops->speed_control(m, s))
#define Motor_TorqueOpenLoop(m, t)     ((m)->ops->torque_control(m, t))
#define Motor_Stop(m)                  ((m)->ops->stop(m))
#define Motor_Enable(m)                ((m)->ops->enable(m))
#define Motor_Disable(m)               ((m)->ops->disable(m))

typedef enum {
    MOTOR_LK,
    MOTOR_EU,
    MOTOR_DM,
    // 后续可以扩展：MOTOR_XXX
} MotorType;

struct MotorDevice;

// 定义操作函数接口（行为接口）
typedef struct {
    void (*get_angle)(struct MotorDevice *motor);
    void (*speed_control)(struct MotorDevice *motor, float speed);
    void (*position_control)(struct MotorDevice *motor, float angleControl, uint16_t maxSpeed);
    void (*torque_control)(struct MotorDevice *motor, int16_t torque);
    void (*stop)(struct MotorDevice *motor);
    void (*enable)(struct MotorDevice *motor);
    void (*disable)(struct MotorDevice *motor);

    // 新增反馈处理函数指针，按命令区分
    void (*handle_get_angle_feedback)(struct MotorDevice *motor, const uint8_t *data, uint8_t len);
    void (*handle_speed_control_feedback)(struct MotorDevice *motor, const uint8_t *data, uint8_t len);
    void (*handle_position_control_feedback)(struct MotorDevice *motor, const uint8_t *data, uint8_t len);
    void (*handle_torque_control_feedback)(struct MotorDevice *motor, const uint8_t *data, uint8_t len);
    void (*handle_stop_feedback)(struct MotorDevice *motor, const uint8_t *data, uint8_t len);
    void (*handle_enable_feedback)(struct MotorDevice *motor, const uint8_t *data, uint8_t len);
    void (*handle_disable_feedback)(struct MotorDevice *motor, const uint8_t *data, uint8_t len);
} MotorOps;

// 电机状态对象定义
typedef struct MotorStatus {
    uint32_t last_heartbeat;        /* 最后心跳时间 */
    motor_mode_t mode;              /* 当前模式 */
    /* 时间戳记录 */
    uint32_t last_master_cmd;   /* 最后主控命令时间 */
    uint32_t last_motor_resp;   /* 最后电机响应时间 */

    /* 电源控制 */
    GPIO_TypeDef* mos_port;     /* MOS管控制端口 */
    uint16_t mos_pin;           /* MOS管控制引脚 */
} MotorStatus;

// 电机设备对象定义
typedef struct MotorDevice {
    const char *name;                 // 电机角色名，如 "left_shoulder"
    MotorType type;                   // 电机类型
    uint32_t can_id;                  // 电机对应的 CAN ID
    CAN_HandleTypeDef *can_bus;       // CAN 句柄
    const MotorOps *ops;              // 行为接口集（函数指针表）

    // 可选字段（便于扩展）
    void *context;                    // 用户上下文，指向配置或状态结构体
    int32_t last_angle;               // 上次角度读取（可选）
    uint8_t enabled;                  // 当前使能状态
    uint8_t fault;                    // 故障标志
} MotorDevice;

#endif //SAFEPOWER_MOTOR_INTERFACE_H
