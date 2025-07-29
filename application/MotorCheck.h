//
// Created by Steins_Xin on 2025/3/31.
//

#ifndef SAFEPOWER_MOTORCHECK_H
#define SAFEPOWER_MOTORCHECK_H
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "gpio.h"
#include "string.h"

/* 应用层 */
#include "MotorDrive.h"
#include "LKMotor.h"
#include "EUMotor.h"

/* ==================== 超时参数定义 ==================== */
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
 * @brief 电机类型枚举
 */
typedef enum {
    UNKNOWN_TYPE,        /* 未知类型 */
    MOTOR_TYPE_LK,       /* LK电机类型 */
    MOTOR_TYPE_EU        /* EU电机类型 */
} motor_type_t;

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

    /* 类型标识 */
    motor_type_t type;          /* 电机类型 */
} motor_controller_t;

/**
 * @brief CAN消息类型枚举
 */
typedef enum {
    MSG_HEARTBEAT,      /* 主控心跳包 */
    MSG_SPEED_MODE_CMD, /* 速度模式命令 */
    MSG_POS_MODE_CMD,   /* 位置模式命令 */
    MSG_SPEED_CMD,      /* 速度命令 */
    MSG_TORQUE_CMD,     /* 力矩命令 */
    MSG_MOTOR_FEEDBACK, /* 电机反馈 */
    MSG_REBOOT_CMD,     /* 重启命令 */
    MSG_UNKNOWN         /* 未知命令 */
} can_msg_type_t;

/**
 * @brief 电机模式枚举
 */
typedef enum {
    UNKNOWN_MODE,       /* 未知模式 */
    SPEED_MODE,         /* 速度模式 */
    POS_MODE,           /* 位置模式 */
} motor_mode_t;

/**
 * @brief 安全控制系统结构体
 */
typedef struct {
    motor_controller_t motors[18];  /* 所有电机控制器(0-6:左臂,7-13:右臂,14-17:腰腿) */
    uint32_t last_heartbeat;        /* 最后心跳时间 */
    motor_mode_t mode;              /* 当前模式 */
    uint8_t lk_enabled;             /* 是否启用LK电机组 */
    uint8_t eu_enabled;             /* 是否启用EU电机组 */
} safety_control_system_t;

/* ==================== 函数声明 ==================== */

/**
 * @brief 获取当前时间戳
 * @return 当前时间戳(ms)
 */
uint32_t get_timer_ticks(void);

/**
 * @brief 初始化电机控制器
 * @param ctrl 电机控制器指针
 * @param can_id CAN ID
 * @param bus CAN总线句柄
 * @param state 初始状态
 * @param port MOS端口
 * @param pin MOS引脚
 * @param type 电机类型
 */
void init_motor_controller(motor_controller_t* ctrl, uint32_t can_id,
                           CAN_HandleTypeDef* bus, motor_state_t state,
                           GPIO_TypeDef* port, uint16_t pin, motor_type_t type);

/**
 * @brief 电机电源控制
 * @param port MOS端口
 * @param pin MOS引脚
 * @param enable 使能标志 (0: 关闭, 1: 开启)
 */
void motor_power_ctrl(GPIO_TypeDef* port, uint16_t pin, uint8_t enable);

/**
 * @brief 处理电机超时
 * @param motor 电机控制器指针
 * @param now 当前时间戳
 */
void handle_timeout(motor_controller_t* motor, uint32_t now);

/**
 * @brief 初始化安全控制系统
 * @param sys 安全控制系统指针
 */
void safety_system_init(safety_control_system_t* sys);

/**
 * @brief 初始化LK电机组
 * @param sys 安全控制系统指针
 */
void init_lk_motors(safety_control_system_t* sys);

/**
 * @brief 初始化EU电机组
 * @param sys 安全控制系统指针
 */
void init_eu_motors(safety_control_system_t* sys);

/**
 * @brief 安全监控任务
 * @param sys 安全控制系统指针
 */
void safety_monitor_task(safety_control_system_t* sys);

/**
 * @brief 解码CAN消息
 * @param std_id CAN标准ID
 * @param data CAN数据
 * @param type 电机类型
 * @return 消息类型
 */
can_msg_type_t decode_can_msg(uint32_t std_id, const uint8_t* data, motor_type_t type);

/**
 * @brief 处理CAN消息
 * @param sys 安全控制系统指针
 * @param std_id CAN标准ID
 * @param data CAN数据
 * @param now 当前时间戳
 */
void process_can_message(safety_control_system_t* sys, uint32_t std_id, const uint8_t* data, uint32_t now);


#endif //SAFEPOWER_MOTORCHECK_H
