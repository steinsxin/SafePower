//
// Created by Steins_Xin on 2025/3/31.
//

#ifndef SAFEPOWER_MOTOR_CHECK_TASK_H
#define SAFEPOWER_MOTOR_CHECK_TASK_H
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "gpio.h"
#include "string.h"

/* 应用层 */
#include "driver/motor_drive_lk.h"
#include "driver/motor_drive_eu.h"
#include "motor_config.h"
#include "system_config.h"

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


#endif //SAFEPOWER_MOTOR_CHECK_TASK_H
