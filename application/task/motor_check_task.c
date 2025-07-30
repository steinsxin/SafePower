#include "motor_check_task.h"

/* ==================== 全局变量 ==================== */
safety_control_system_t system;

/* ==================== 函数实现 ==================== */

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
                           GPIO_TypeDef* port, uint16_t pin)
{
    if (ctrl == NULL) return;

    ctrl->can_id = can_id;
    ctrl->bus = bus;
    ctrl->state = state;
    ctrl->retries = 0;
    ctrl->last_master_cmd = 0;
    ctrl->last_motor_resp = 0;
    ctrl->mos_port = port;
    ctrl->mos_pin = pin;
}

/**
 * @brief 电机电源控制
 * @param port MOS端口
 * @param pin MOS引脚
 * @param enable 使能标志 (0: 关闭, 1: 开启)
 */
void motor_power_ctrl(GPIO_TypeDef* port, uint16_t pin, uint8_t enable)
{
    if (port == NULL) return;
    HAL_GPIO_WritePin(port, pin, enable ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/**
 * @brief 处理电机超时
 * @param motor 电机控制器指针
 * @param now 当前时间戳
 */
void handle_timeout(motor_controller_t* motor, uint32_t now)
{
    if (motor == NULL) return;

    /* 检查命令和响应超时 */
    uint8_t cmd_timeout = (now - motor->last_master_cmd) > MASTER_CMD_TIMEOUT_MS;
    uint8_t resp_timeout = (now - motor->last_motor_resp) > MOTOR_RESP_TIMEOUT_MS;

    if (cmd_timeout || resp_timeout) {
        if (motor->retries < MAX_RETRY_ATTEMPTS) {
            /* 发送零速命令并重试 */
//            if (motor->type == MOTOR_TYPE_EU) {
////                EU_Speed_CMD(motor->bus, motor->can_id, 0);
//                vTaskDelay(2);
////                EU_Get_Angle_CMD(motor->bus, motor->can_id);
//                vTaskDelay(2);
//            } else {
////                LK_Speed_CMD(motor->bus, motor->can_id, 0);
//                vTaskDelay(1);
//            }

            motor->retries++;
            motor->state = MOTOR_RETRY_PENDING;
        } else {
            /* 重试次数耗尽，执行断电操作 */
//            if (motor->type == MOTOR_TYPE_EU) {
//                /* EU电机需要断开两个MOS */
//                motor_power_ctrl(MOS1_GPIO_Port, MOS1_Pin, 0);
//                motor_power_ctrl(MOS2_GPIO_Port, MOS2_Pin, 0);
//            } else{
//                motor_power_ctrl(motor->mos_port, motor->mos_pin, 0);
//            }

            motor->state = MOTOR_CRITICAL_FAIL;
        }
    }
}

/**
 * @brief 初始化安全控制系统
 * @param sys 安全控制系统指针
 */
void safety_system_init(safety_control_system_t* sys)
{
    if (sys == NULL) return;
    memset(sys, 0, sizeof(safety_control_system_t));
    sys->lk_enabled = 0;
    sys->eu_enabled = 0;
}

/**
 * @brief 初始化LK电机组
 * @param sys 安全控制系统指针
 */
void init_lk_motors(safety_control_system_t* sys)
{
    /* 初始化左臂7个电机 */
    for (int i = 0; i < 7; i++) {
        init_motor_controller(&sys->motors[i], 0x145 + i, &hcan1, MOTOR_INIT,
                              MOS1_GPIO_Port, MOS1_Pin);
    }

    /* 初始化右臂7个电机 */
    for (int i = 0; i < 7; i++) {
        init_motor_controller(&sys->motors[i+7], 0x14D + i, &hcan2, MOTOR_INIT,
                              MOS2_GPIO_Port, MOS2_Pin);
    }
    sys->lk_enabled = 1;
}

/**
 * @brief 初始化EU电机组
 * @param sys 安全控制系统指针
 */
void init_eu_motors(safety_control_system_t* sys)
{
    /* 初始化腰腿4个电机 */
    for (int i = 0; i < 4; i++) {
        init_motor_controller(&sys->motors[i+14], 0x15 + i, &hcan1, MOTOR_INIT,
                              MOS1_GPIO_Port, MOS1_Pin);
    }
    sys->eu_enabled = 1;
}

/**
 * @brief 安全监控任务
 * @param sys 安全控制系统指针
 */
void safety_monitor_task(safety_control_system_t* sys)
{
    if (sys == NULL) return;

    uint32_t now = get_timer_ticks();
    /* 根据启用的电机组确定需要监控的电机范围 */
    int start_idx = 0;
    int end_idx = 0;

    if (sys->lk_enabled) {
        // LK 电机（0-13）
        start_idx = 0;
        end_idx = 14;
    } else if (sys->eu_enabled) {
        // EU 电机（14-17）
        start_idx = 14;
        end_idx = 18;
    }

    /* 检查所有电机的状态 */
    for (int i = start_idx; i < end_idx; i++) {
        if (sys->motors[i].state == MOTOR_OPERATIONAL ||
            sys->motors[i].state == MOTOR_RETRY_PENDING) {
            handle_timeout(&sys->motors[i], now);
        }
    }
}

/**
 * @brief 电机检查主任务
 * @param argument 任务参数
 */
void motor_check_task(void const* argument)
{
    LED_ON(LED1);
    safety_system_init(&system);
    while (1) {
//        safety_monitor_task(&system);
        vTaskDelay(5);  /* 5ms周期监控 */
    }
}