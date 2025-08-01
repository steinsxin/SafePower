//
// Created by Steins_Xin on 2025/7/29.
//

#include "motor_drive_lk.h"

/**
 * @brief 多圈角度命令（0x92）
 *
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void LK_GetMultiTurnAngle(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID){
    uint8_t data[8];
    memset(data, 0, sizeof(data));  // 全部初始化为 0

    data[0] = LK_CMD_GET_MULTI_TURN_ANGLE;  // 设置命令字节

    CAN_Send(CAN_BUS, CAN_ID, data, 8);     // 发送完整8字节
}

/**
 * @brief 处理读取多圈角度命令的反馈数据
 * @param motor 指向对应的电机对象
 * @param data 指向接收到的CAN数据帧（8字节）
 * @param len 数据长度
 */
void LK_HandleMultiTurnAngleFeedback(MotorDevice *motor, const uint8_t *data, uint8_t len){
    if (len < 8 || data[0] != LK_CMD_GET_MULTI_TURN_ANGLE) {
        return;  // 非法数据
    }

    int64_t raw_angle = 0;

    // 组装 7 字节有符号整数（小端）
    raw_angle |= ((int64_t)data[1]) << 0;
    raw_angle |= ((int64_t)data[2]) << 8;
    raw_angle |= ((int64_t)data[3]) << 16;
    raw_angle |= ((int64_t)data[4]) << 24;
    raw_angle |= ((int64_t)data[5]) << 32;
    raw_angle |= ((int64_t)data[6]) << 40;
    raw_angle |= ((int64_t)(int8_t)data[7]) << 48;  // 使用 int8_t 保留符号扩展

    // 存储角度（单位：度）
    motor->status.current_angle_deg = (double)raw_angle / 100.0;
}

/**
 * @brief 单圈角度命令（0x94）
 *
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void LK_GetSingleTurnAngle(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID){
    uint8_t data[8];
    memset(data, 0, sizeof(data));  // 全部初始化为 0

    data[0] = LK_CMD_GET_SINGLE_TURN_ANGLE;  // 设置命令字节

    CAN_Send(CAN_BUS, CAN_ID, data, 8);     // 发送完整8字节
}

/**
 * @brief 处理读取单圈角度命令的反馈数据
 * @param motor 指向对应的电机对象
 * @param data 指向接收到的CAN数据帧（8字节）
 * @param len 数据长度
 */
void LK_HandleSingleTurnAngleFeedback(MotorDevice *motor, const uint8_t *data, uint8_t len){
    if (len < 8 || data[0] != LK_CMD_GET_SINGLE_TURN_ANGLE) {
        return;  // 非法数据或命令字不匹配
    }

    uint32_t raw_circle_angle = 0;

    // 小端解析：低位在低地址
    raw_circle_angle |= ((uint32_t)data[4]) << 0;
    raw_circle_angle |= ((uint32_t)data[5]) << 8;
    raw_circle_angle |= ((uint32_t)data[6]) << 16;
    raw_circle_angle |= ((uint32_t)data[7]) << 24;

    // 存储角度（单位：度）
    motor->status.current_angle_deg = (double)raw_circle_angle / 100.0;
}

/**
 * @brief 状态1 + 错误标志命令（0x9A）
 *
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void LK_GetStatus1AndError(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID){
    uint8_t data[8];
    memset(data, 0, sizeof(data));  // 全部初始化为 0

    data[0] = LK_CMD_GET_STATUS_1_AND_ERROR;  // 设置命令字节

    CAN_Send(CAN_BUS, CAN_ID, data, 8);     // 发送完整8字节
}

/**
 * @brief 处理读取状态1与错误标志命令的反馈数据
 * @param motor 指向对应的电机对象
 * @param data 指向接收到的CAN数据帧（8字节）
 * @param len 数据长度
 */
void LK_HandleStatus1AndErrorFeedback(MotorDevice *motor, const uint8_t *data, uint8_t len){
    if (len < 8 || data[0] != LK_CMD_GET_STATUS_1_AND_ERROR) {
        return;  // 非法数据
    }

    // 解析电机温度（int8_t -> int16_t，防止负温度时转为大正数）
    int8_t temp_raw = (int8_t)data[1];
    motor->status.temperature_celsius = (int16_t)temp_raw;

    // 解析电压（单位 0.1V）
    uint16_t voltage_raw = ((uint16_t)data[4] << 8) | (uint16_t)data[3];
    motor->status.voltage = voltage_raw;

    // 错误标志
    motor->status.fault_code = data[7];
}

/**
 * @brief 清除错误标志命令（0x9B）
 *
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void LK_ClearErrorFlag(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID){
    uint8_t data[8];
    memset(data, 0, sizeof(data));  // 全部初始化为 0

    data[0] = LK_CMD_CLEAR_ERROR_FLAG;  // 设置命令字节

    CAN_Send(CAN_BUS, CAN_ID, data, 8);     // 发送完整8字节
}

/**
 * @brief 处理清除错误标志命令的反馈数据
 * @param motor 指向对应的电机对象
 * @param data 指向接收到的CAN数据帧（8字节）
 * @param len 数据长度
 */
void LK_HandleClearErrorFlagFeedback(MotorDevice *motor, const uint8_t *data, uint8_t len){
    if (len < 8 || data[0] != LK_CMD_CLEAR_ERROR_FLAG) {
        return;  // 非法数据
    }

    // 解析电机温度（int8_t -> int16_t，防止负温度时转为大正数）
    int8_t temp_raw = (int8_t)data[1];
    motor->status.temperature_celsius = (int16_t)temp_raw;

    // 解析电压（单位 0.1V）
    uint16_t voltage_raw = ((uint16_t)data[4] << 8) | (uint16_t)data[3];
    motor->status.voltage = voltage_raw;

    // 错误标志
    motor->status.fault_code = data[7];
}

/**
 * @brief 状态2命令（0x9C）
 *
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void LK_GetStatus2(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID){
    uint8_t data[8];
    memset(data, 0, sizeof(data));  // 全部初始化为 0

    data[0] = LK_CMD_GET_STATUS_2;  // 设置命令字节

    CAN_Send(CAN_BUS, CAN_ID, data, 8);     // 发送完整8字节
}

/**
 * @brief 处理读取状态2命令的反馈数据
 * @param motor 指向对应的电机对象
 * @param data 指向接收到的CAN数据帧（8字节）
 * @param len 数据长度
 */
void LK_HandleStatus2Feedback(MotorDevice *motor, const uint8_t *data, uint8_t len){
    if (len < 8 || data[0] != LK_CMD_GET_STATUS_2) {
        return; // 非法数据
    }

    // 解析电机温度（int8_t）
    int8_t temp_raw = (int8_t)data[1];
    motor->status.temperature_celsius = (int16_t)temp_raw;

    // 解析转矩电流（int16_t，小端）
    int16_t iq_raw = (int16_t)(data[2] | (data[3] << 8));
    motor->status.current_iq = iq_raw;

    // 解析电机速度（int16_t，小端，单位：dps）
    int16_t speed_raw = (int16_t)(data[4] | (data[5] << 8));
    motor->status.current_speed_rpm = speed_raw;

    // 解析编码器位置（uint16_t，小端，14bit有效）
    uint16_t encoder_raw = (uint16_t)(data[6] | (data[7] << 8));
    encoder_raw &= 0x3FFF;  // 取低14位有效
    motor->status.encoder = encoder_raw;
}

/**
 * @brief 状态3命令（0x9D）
 *
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void LK_GetStatus3(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID){
    uint8_t data[8];
    memset(data, 0, sizeof(data));  // 全部初始化为 0

    data[0] = LK_CMD_GET_STATUS_3;  // 设置命令字节

    CAN_Send(CAN_BUS, CAN_ID, data, 8);     // 发送完整8字节
}

/**
 * @brief 处理读取状态3命令的反馈数据
 * @param motor 指向对应的电机对象
 * @param data 指向接收到的CAN数据帧（8字节）
 * @param len 数据长度
 */
void LK_HandleStatus3Feedback(MotorDevice *motor, const uint8_t *data, uint8_t len){
    if (len < 8 || data[0] != LK_CMD_GET_STATUS_3) {
        return; // 非法数据
    }

    // 解析电机温度（int8_t）
    int8_t temp_raw = (int8_t)data[1];
    motor->status.temperature_celsius = (int16_t)temp_raw;

    // 解析A相电流（int16_t，小端）
    int16_t iA_raw = (int16_t)(data[2] | (data[3] << 8));

    // 解析B相电流（int16_t，小端）
    int16_t iB_raw = (int16_t)(data[4] | (data[5] << 8));

    // 解析C相电流（int16_t，小端）
    int16_t iC_raw = (int16_t)(data[6] | (data[7] << 8));
}

/**
 * @brief 转矩开环控制命令（0xA0）
 *
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 * @param torqueValue 转矩值
 */
void LK_TorqueOpenLoop(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID, int16_t torqueValue){
    // 限幅
    if (torqueValue > LK_TORQUE_OPEN_LOOP_MAX) {
        torqueValue = LK_TORQUE_OPEN_LOOP_MAX;
    } else if (torqueValue < LK_TORQUE_OPEN_LOOP_MIN) {
        torqueValue = LK_TORQUE_OPEN_LOOP_MIN;
    }

    uint8_t data[8];
    memset(data, 0, sizeof(data));  // 初始化为0

    data[0] = LK_CMD_TORQUE_OPEN_LOOP;
    data[4] = (uint8_t)(torqueValue & 0xFF);         // 低字节
    data[5] = (uint8_t)((torqueValue >> 8) & 0xFF);  // 高字节

    CAN_Send(CAN_BUS, CAN_ID, data, 8);
}

/**
 * @brief 处理转矩开环控制命令的反馈数据
 * @param motor 指向对应的电机对象
 * @param data 指向接收到的CAN数据帧（8字节）
 * @param len 数据长度
 */
void LK_HandleTorqueOpenLoopFeedback(MotorDevice *motor, const uint8_t *data, uint8_t len){
    if (len < 8 || data[0] != LK_CMD_TORQUE_OPEN_LOOP) {
        return; // 非法数据
    }
}

/**
 * @brief 转矩闭环控制命令（0xA1）
 *
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 * @param torqueValue 转矩值
 */
void LK_TorqueClosedLoop(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID, int16_t torqueValue){
    // 限幅
    if (torqueValue > LK_TORQUE_CLOSED_LOOP_MAX) {
        torqueValue = LK_TORQUE_CLOSED_LOOP_MAX;
    } else if (torqueValue < LK_TORQUE_CLOSED_LOOP_MIN) {
        torqueValue = LK_TORQUE_CLOSED_LOOP_MIN;
    }

    uint8_t data[8] = {0};
    data[0] = LK_CMD_TORQUE_CLOSED_LOOP;

    data[4] = (uint8_t)(torqueValue & 0xFF);         // 低字节
    data[5] = (uint8_t)((torqueValue >> 8) & 0xFF);  // 高字节

    CAN_Send(CAN_BUS, CAN_ID, data, 8);
}

/**
 * @brief 处理转矩闭环控制命令的反馈数据
 * @param motor 指向对应的电机对象
 * @param data 指向接收到的CAN数据帧（8字节）
 * @param len 数据长度
 */
void LK_HandleTorqueClosedLoopFeedback(MotorDevice *motor, const uint8_t *data, uint8_t len){
    if (len < 8 || data[0] != LK_CMD_TORQUE_CLOSED_LOOP) {
        return; // 非法数据
    }

    // 解析电机温度
    int8_t temp_raw = (int8_t)data[1];
    motor->status.temperature_celsius = (int16_t)temp_raw;

    // 解析转矩电流 iq（小端）
    int16_t iq_raw = (int16_t)(data[2] | (data[3] << 8));
    motor->status.current_iq = iq_raw;

    // 解析电机速度 speed（小端）
    int16_t speed_raw = (int16_t)(data[4] | (data[5] << 8));
    motor->status.current_speed_rpm = speed_raw;

    // 解析编码器位置 encoder（小端，14bit有效）
    uint16_t encoder_raw = (uint16_t)(data[6] | (data[7] << 8));
    encoder_raw &= 0x3FFF;  // 取低14位有效
    motor->status.encoder = encoder_raw;
}

/**
 * @brief 速度控制命令（0xA2）
 *
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 * @param speedControl 速度值（单位：rpm）
 */
void LK_SpeedControl(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID, float speedControl){
    uint8_t data[8] = {0};

    data[0] = LK_CMD_SPEED_CONTROL;

    // 将 RPM 转换为协议单位（0.01°/s per LSB）
    int32_t rpm = (int32_t)(speedControl * 600.0f);

    // 拆分 speedControl 成 4 个字节，低位在前（小端序）
    data[4] = (uint8_t)(rpm & 0xFF);
    data[5] = (uint8_t)((rpm >> 8) & 0xFF);
    data[6] = (uint8_t)((rpm >> 16) & 0xFF);
    data[7] = (uint8_t)((rpm >> 24) & 0xFF);

    CAN_Send(CAN_BUS, CAN_ID, data, 8);
}

/**
 * @brief 处理速度控制命令的反馈数据
 * @param motor 指向对应的电机对象
 * @param data 指向接收到的CAN数据帧（8字节）
 * @param len 数据长度
 */
void LK_HandleSpeedControlFeedback(MotorDevice *motor, const uint8_t *data, uint8_t len){
    if (len < 8 || data[0] != LK_CMD_SPEED_CONTROL) {
        return; // 非法数据
    }

    // 电机温度
    int8_t temp_raw = (int8_t)data[1];
    motor->status.temperature_celsius = (int16_t)temp_raw;

    // 转矩电流 iq（小端）
    int16_t iq_raw = (int16_t)(data[2] | (data[3] << 8));
    motor->status.current_iq = iq_raw;

    // 转速 speed（小端）
    int16_t speed_raw = (int16_t)(data[4] | (data[5] << 8));
    motor->status.current_speed_rpm = speed_raw;

    // 编码器位置 encoder（小端，14bit有效）
    uint16_t encoder_raw = (uint16_t)(data[6] | (data[7] << 8));
    encoder_raw &= 0x3FFF;  // 清除高2位，仅保留14位有效数据
    motor->status.encoder = encoder_raw;
}

/**
 * @brief 多圈位置控制命令（0xA4）
 *
 * @param CAN_BUS      CAN总线句柄
 * @param CAN_ID       电机ID
 * @param angleControl 位置控制值（单位0.01度/LSB，int32_t）
 * @param maxSpeed     最大速度限制（单位度/秒LSB，uint16_t）
 */
void LK_PositionControlMulti(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID, float angleControl, uint16_t maxSpeed) {
    uint8_t data[8];
    memset(data, 0, sizeof(data));

    // 角度转为 0.01度单位整数
    int32_t angle = (int32_t)(angleControl * 100.0f);

    data[0] = LK_CMD_POSITION_CONTROL_MULTI; // 0xA4
    data[1] = 0x00;                          // 保留

    data[2] = (uint8_t)(maxSpeed & 0xFF);
    data[3] = (uint8_t)((maxSpeed >> 8) & 0xFF);

    data[4] = (uint8_t)(angle & 0xFF);
    data[5] = (uint8_t)((angle >> 8) & 0xFF);
    data[6] = (uint8_t)((angle >> 16) & 0xFF);
    data[7] = (uint8_t)((angle >> 24) & 0xFF);

    CAN_Send(CAN_BUS, CAN_ID, data, 8);
}

/**
 * @brief 处理电机多圈命令的反馈数据
 * @param motor 指向对应的电机对象
 * @param data 指向接收到的CAN数据帧（8字节）
 * @param len 数据长度
 */
void LK_HandlePositionControlMultiFeedback(MotorDevice *motor, const uint8_t *data, uint8_t len){
    if (len < 8 || data[0] != LK_CMD_POSITION_CONTROL_MULTI) {
        return; // 非法数据
    }

    // 电机温度
    int8_t temp_raw = (int8_t)data[1];
    motor->status.temperature_celsius = (int16_t)temp_raw;

    // 转矩电流 iq（小端）
    int16_t iq_raw = (int16_t)(data[2] | (data[3] << 8));
    motor->status.current_iq = iq_raw;

    // 转速 speed（小端）
    int16_t speed_raw = (int16_t)(data[4] | (data[5] << 8));
    motor->status.current_speed_rpm = speed_raw;

    // 编码器位置 encoder（小端，14bit有效）
    uint16_t encoder_raw = (uint16_t)(data[6] | (data[7] << 8));
    encoder_raw &= 0x3FFF;  // 保留14位有效数据
    motor->status.encoder = encoder_raw;
}

/**
 * @brief 单圈位置控制命令（0xA6）
 *
 * @param CAN_BUS       CAN总线句柄
 * @param CAN_ID        电机ID
 * @param angleDegrees  目标角度，单位 度，范围 [0, 359.99]
 * @param spinDirection 旋转方向，0x00顺时针，0x01逆时针
 * @param maxSpeed      最大速度限制，单位 度/秒
 */
void LK_PositionControlSingle(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID, float angleControl, uint8_t spinDirection, uint16_t maxSpeed){
    uint8_t data[8];
    memset(data, 0, sizeof(data));

    // 限制角度范围在0~359.99度
    if (angleControl < 0) angleControl = 0;
    if (angleControl >= 360) angleControl = 359.99f;

    // 角度转为0.01度单位整数
    uint16_t angle = (uint16_t)(angleControl * 100.0f);

    data[0] = LK_CMD_POSITION_CONTROL_SINGLE;  // 命令字 0xA3 (示例)
    data[1] = spinDirection;                    // 转动方向 0x00顺时针，0x01逆时针

    data[2] = (uint8_t)(maxSpeed & 0xFF);      // 速度低字节
    data[3] = (uint8_t)((maxSpeed >> 8) & 0xFF);// 速度高字节

    data[4] = (uint8_t)(angle & 0xFF);  // 角度低字节
    data[5] = (uint8_t)((angle >> 8) & 0xFF); // 角度高字节

    // data[6], data[7] 保留为0

    CAN_Send(CAN_BUS, CAN_ID, data, 8);
}

/**
 * @brief 处理电机单圈命令的反馈数据
 * @param motor 指向对应的电机对象
 * @param data 指向接收到的CAN数据帧（8字节）
 * @param len 数据长度
 */
void LK_HandlePositionControlSingleFeedback(MotorDevice *motor, const uint8_t *data, uint8_t len){
    if (len < 8 || data[0] != LK_CMD_POSITION_CONTROL_SINGLE) {
        return; // 非法数据
    }

    // 电机温度
    int8_t temp_raw = (int8_t)data[1];
    motor->status.temperature_celsius = (int16_t)temp_raw;

    // 转矩电流 iq（小端）
    int16_t iq_raw = (int16_t)(data[2] | (data[3] << 8));
    motor->status.current_iq = iq_raw;

    // 转速 speed（小端）
    int16_t speed_raw = (int16_t)(data[4] | (data[5] << 8));
    motor->status.current_speed_rpm = speed_raw;

    // 编码器位置 encoder（小端，14bit有效）
    uint16_t encoder_raw = (uint16_t)(data[6] | (data[7] << 8));
    encoder_raw &= 0x3FFF;  // 保留14位有效数据
    motor->status.encoder = encoder_raw;
}

/**
 * @brief 发送电机停止命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void LK_StopMotor(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID){
    uint8_t data[8];
    memset(data, 0, sizeof(data));  // 全部初始化为 0

    data[0] = LK_CMD_STOP;  // 设置命令字节

    CAN_Send(CAN_BUS, CAN_ID, data, 8);     // 发送完整8字节
}

/**
 * @brief 处理电机停止命令的反馈数据
 * @param motor 指向对应的电机对象
 * @param data 指向接收到的CAN数据帧（8字节）
 * @param len 数据长度
 */
void LK_HandleStopMotorFeedback(MotorDevice *motor, const uint8_t *data, uint8_t len){
    if (len < 8 || data[0] != LK_CMD_STOP) {
        return; // 非法数据
    }
}

/**
 * @brief 发送电机关闭命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void LK_DisableMotor(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID){
    uint8_t data[8];
    memset(data, 0, sizeof(data));  // 全部初始化为 0

    data[0] = LK_CMD_DISABLE;  // 设置命令字节

    CAN_Send(CAN_BUS, CAN_ID, data, 8);     // 发送完整8字节
}

/**
 * @brief 处理电机关闭命令的反馈数据
 * @param motor 指向对应的电机对象
 * @param data 指向接收到的CAN数据帧（8字节）
 * @param len 数据长度
 */
void LK_HandleDisableMotorFeedback(MotorDevice *motor, const uint8_t *data, uint8_t len){
    if (len < 8 || data[0] != LK_CMD_DISABLE) {
        return; // 非法数据
    }
}

/**
 * @brief 发送电机使能（运行）命令
 * @param CAN_BUS CAN总线句柄
 * @param CAN_ID 电机ID
 */
void LK_EnableMotor(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID){
    uint8_t data[8];
    memset(data, 0, sizeof(data));  // 全部初始化为 0

    data[0] = LK_CMD_ENABLE;  // 设置命令字节

    CAN_Send(CAN_BUS, CAN_ID, data, 8);     // 发送完整8字节
}

/**
 * @brief 处理电机使能（运行）命令的反馈数据
 * @param motor 指向对应的电机对象
 * @param data 指向接收到的CAN数据帧（8字节）
 * @param len 数据长度
 */
void LK_HandleEnableMotorFeedback(MotorDevice *motor, const uint8_t *data, uint8_t len){
    if (len < 8 || data[0] != LK_CMD_ENABLE) {
        return; // 非法数据
    }
}