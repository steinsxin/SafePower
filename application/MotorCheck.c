#include "MotorCheck.h"

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
                           GPIO_TypeDef* port, uint16_t pin, motor_type_t type)
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
    ctrl->type = type;
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
            if (motor->type == MOTOR_TYPE_EU) {
                EU_Speed_CMD(motor->bus, motor->can_id, 0);
                vTaskDelay(2);
                EU_Get_Angle_CMD(motor->bus, motor->can_id);
                vTaskDelay(2);
            } else {
                LK_Speed_CMD(motor->bus, motor->can_id, 0);
                vTaskDelay(1);
            }

            motor->retries++;
            motor->state = MOTOR_RETRY_PENDING;
        } else {
            /* 重试次数耗尽，执行断电操作 */
            if (motor->type == MOTOR_TYPE_EU) {
                /* EU电机需要断开两个MOS */
                motor_power_ctrl(MOS1_GPIO_Port, MOS1_Pin, 0);
                motor_power_ctrl(MOS2_GPIO_Port, MOS2_Pin, 0);
            } else{
                motor_power_ctrl(motor->mos_port, motor->mos_pin, 0);
            }

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
    sys->mode = UNKNOWN_MODE;
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
                              MOS1_GPIO_Port, MOS1_Pin, MOTOR_TYPE_LK);
    }

    /* 初始化右臂7个电机 */
    for (int i = 0; i < 7; i++) {
        init_motor_controller(&sys->motors[i+7], 0x14D + i, &hcan2, MOTOR_INIT,
                              MOS2_GPIO_Port, MOS2_Pin, MOTOR_TYPE_LK);
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
                              MOS1_GPIO_Port, MOS1_Pin, MOTOR_TYPE_EU);
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
 * @brief 解码CAN消息
 * @param std_id CAN标准ID
 * @param data CAN数据
 * @param type 电机类型
 * @return 消息类型
 */
can_msg_type_t decode_can_msg(uint32_t std_id, const uint8_t* data, motor_type_t type)
{
    if (data == NULL) return MSG_UNKNOWN;

    /* 通用消息处理 */
    if (std_id == 0x100 && data[0] == 0xAA) return MSG_HEARTBEAT;
    if (std_id == 0x101 && data[0] == 0x55) return MSG_REBOOT_CMD;

    /* 类型特定消息处理 */
    if (type == MOTOR_TYPE_LK) {
        /* LK电机速度命令 */
        if ((std_id >= 0x145 && std_id <= 0x154) && data[0] == 0xA2) {
            return (data[1] == 0x00) ? MSG_SPEED_CMD : MSG_MOTOR_FEEDBACK;
        }
        /* LK电机力矩命令 */
        if ((std_id >= 0x145 && std_id <= 0x154) && data[0] == 0xA1) {
            return (data[1] == 0x00) ? MSG_TORQUE_CMD : MSG_MOTOR_FEEDBACK;
        }
    }
    else if (type == MOTOR_TYPE_EU) {
        /* EU电机模式切换命令 */
        if ((std_id >= 0x15 && std_id <= 0x18) && data[0] == 0x01) {
            if (data[1] == 15 && data[5] == 0x03) return MSG_SPEED_MODE_CMD;
            if (data[1] == 15 && data[5] == 0x01) return MSG_POS_MODE_CMD;
        }
        /* EU电机速度命令 */
        if ((std_id >= 0x15 && std_id <= 0x18) && data[0] == 0x05 && data[1] == 0x09) {
            return MSG_SPEED_CMD;
        }
        /* EU电机反馈 */
        if ((std_id >= 0x15 && std_id <= 0x18) && data[0] == 0x04 && data[1] == 0x07) {
            return MSG_MOTOR_FEEDBACK;
        }
    }

    return MSG_UNKNOWN;
}

/**
 * @brief 处理CAN消息
 * @param sys 安全控制系统指针
 * @param std_id CAN标准ID
 * @param data CAN数据
 * @param now 当前时间戳
 */
void process_can_message(safety_control_system_t* sys, uint32_t std_id, const uint8_t* data, uint32_t now)
{
    /* 确定消息类型 */
    motor_type_t type = UNKNOWN_TYPE;
    if (std_id >= LEFT_ARM_START_ID && std_id <= RIGHT_ARM_END_ID) type = MOTOR_TYPE_LK;
    else if (std_id >= WAIST_LEG_START_ID && std_id <= WAIST_LEG_END_ID) type = MOTOR_TYPE_EU;

    /* 按需初始化电机组 */
    if (type == MOTOR_TYPE_LK && !sys->lk_enabled) {
        init_lk_motors(sys);
    } else if (type == MOTOR_TYPE_EU && !sys->eu_enabled) {
        init_eu_motors(sys);
    }

    /* 解码消息类型 */
    can_msg_type_t msg_type = decode_can_msg(std_id, data, type);

    /* 处理不同类型的消息 */
    switch (msg_type) {
        case MSG_HEARTBEAT:
            /* 更新心跳时间 */
            sys->last_heartbeat = now;
            break;

        case MSG_REBOOT_CMD:
            /* 系统重启 */
            safety_system_init(sys);
            motor_power_ctrl(MOS1_GPIO_Port, MOS1_Pin, 1);
            motor_power_ctrl(MOS2_GPIO_Port, MOS2_Pin, 1);
            break;

        case MSG_SPEED_MODE_CMD:
            /* 切换到速度模式 */
            sys->mode = SPEED_MODE;
            break;

        case MSG_POS_MODE_CMD:
            /* 切换到位置模式 */
            sys->mode = POS_MODE;
            break;

        case MSG_SPEED_CMD:
        case MSG_TORQUE_CMD:
            /* 更新主控命令时间戳 */
            if (type == MOTOR_TYPE_LK) {
                // LK 电机（左右臂 0-13）
                for (int i = 0; i < 14; i++) {
                    if (sys->motors[i].can_id == std_id) {
                        sys->motors[i].last_master_cmd = now;
                        sys->motors[i].state = MOTOR_OPERATIONAL;
                    }
                }
            } else if (type == MOTOR_TYPE_EU) {
                // EU 电机（腰腿 14-17）
                for (int i = 14; i < 18; i++) {
                    if (sys->motors[i].can_id == std_id && sys->mode == SPEED_MODE) {
                        sys->motors[i].last_master_cmd = now;
                        sys->motors[i].state = MOTOR_OPERATIONAL;
                    }
                }
            }
            break;

        case MSG_MOTOR_FEEDBACK:
            /* 更新电机响应时间 */
            if (type == MOTOR_TYPE_LK) {
                // LK 电机（左右臂 0-13）
                for (int i = 0; i < 14; i++) {
                    if (sys->motors[i].can_id == std_id) {
                        sys->motors[i].last_motor_resp = now;
                        sys->motors[i].retries = 0;
                        if (sys->motors[i].state == MOTOR_RETRY_PENDING) {
                            sys->motors[i].state = MOTOR_INIT;
                        }
                    }
                }
            } else if (type == MOTOR_TYPE_EU) {
                // EU 电机（腰腿 14-17）
                for (int i = 14; i < 18; i++) {
                    if (sys->motors[i].can_id == std_id) {
                        sys->motors[i].last_motor_resp = now;
                        sys->motors[i].retries = 0;
                        if (sys->motors[i].state == MOTOR_RETRY_PENDING) {
                            sys->motors[i].state = MOTOR_INIT;
                        }
                    }
                }
            }
            break;

        default:
            /* 未知消息不做处理 */
            break;
    }
}

/**
 * @brief CAN接收中断回调函数
 * @param hcan CAN句柄指针
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    uint8_t data[8];
    CAN_RxHeaderTypeDef header;

    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &header, data) == HAL_OK) {
        process_can_message(&system, header.StdId, data, get_timer_ticks());
    }
}

/**
 * @brief 电机检查主任务
 * @param argument 任务参数
 */
void motor_check_task(void const* argument)
{
    safety_system_init(&system);
    while (1) {
        safety_monitor_task(&system);
        vTaskDelay(5);  /* 5ms周期监控 */
    }
}