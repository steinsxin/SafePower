//
// Created by Steins_Xin on 2025/7/29.
//

#ifndef SAFEPOWER_SYSTEM_CONFIG_H
#define SAFEPOWER_SYSTEM_CONFIG_H

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "gpio.h"

/* ==================== 超时参数定义 ==================== */
#define SYSTEM_ID 0x100
#define MASTER_CMD_TIMEOUT_MS     50  /* 主控命令超时(ms) */
#define MOTOR_RESP_TIMEOUT_MS     50  /* 电机响应超时(ms) */
#define MAX_RETRY_ATTEMPTS        5   /* 最大重试次数 */

/* ==================== 类型定义 ==================== */

/**
 * @brief 电机状态枚举
 */
typedef enum {
    MOTOR_INIT,          /* 初始化状态 */
    MOTOR_STANDBY,       /* 就绪状态 */
    MOTOR_OPERATIONAL,   /* 正常运行 */
    MOTOR_RETRY_PENDING, /* 重试等待 */
    MOTOR_CRITICAL_FAIL  /* 严重故障 */
} motor_state_t;


/**
 * @brief 电机控制器结构体
 */
typedef struct {
    /* 基础信息 */
    uint32_t can_id;            /* 电机CAN ID */
    CAN_HandleTypeDef* bus;     /* 所属CAN总线 */

    /* 状态管理 */
    motor_state_t state;        /* 当前状态 */
    uint8_t retries;            /* 重试计数器 */

    /* 时间戳记录 */
    uint32_t last_master_cmd;   /* 最后主控命令时间 */
    uint32_t last_motor_resp;   /* 最后电机响应时间 */

    /* 电源控制 */
    GPIO_TypeDef* mos_port;     /* MOS管控制端口 */
    uint16_t mos_pin;           /* MOS管控制引脚 */

} motor_controller_t;

/**
 * @brief 系统命令枚举
 */
typedef enum {
    System_CMD_Start = 0x01,        /* 系统启动命令 */
    System_CMD_Stop = 0x02,         /* 系统关闭命令 */
    System_CMD_Reset = 0x03         /* 系统重启命令 */
} System_CMD_ID;

/**
 * @brief 安全控制系统结构体
 */
typedef struct {
    motor_controller_t motors[18];  /* 所有电机控制器(0-6:左臂,7-13:右臂,14-17:腰腿) */
    uint32_t last_heartbeat;        /* 最后心跳时间 */
    uint8_t lk_enabled;             /* 是否启用LK电机组 */
    uint8_t eu_enabled;             /* 是否启用EU电机组 */
} safety_control_system_t;

#endif //SAFEPOWER_SYSTEM_CONFIG_H
