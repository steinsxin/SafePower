//
// Created by Steins_Xin on 2025/7/29.
//

#ifndef SAFEPOWER_MOTOR_DRIVE_LK_H
#define SAFEPOWER_MOTOR_DRIVE_LK_H

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include <string.h>
#include "can.h"

/* 应用层 */
#include "can_handle.h"
#include "motor_interface.h"

#define LK_TORQUE_OPEN_LOOP_MIN     (-1000)
#define LK_TORQUE_OPEN_LOOP_MAX     (1000)

#define LK_TORQUE_CLOSED_LOOP_MIN   (-2000)
#define LK_TORQUE_CLOSED_LOOP_MAX   (2000)

// ==================== 命令 ID ====================
/**
 * @brief LK 电机命令枚举
 */
typedef enum {
    LK_CMD_GET_MULTI_TURN_ANGLE = 0x92,           ///< 读取多圈角度
    LK_CMD_GET_SINGLE_TURN_ANGLE = 0x94,          ///< 读取单圈角度
    LK_CMD_GET_STATUS_1_AND_ERROR = 0x9A,         ///< 读取电机状态1与错误标志
    LK_CMD_CLEAR_ERROR_FLAG = 0x9B,               ///< 清除错误标志
    LK_CMD_GET_STATUS_2 = 0x9C,                   ///< 读取电机状态2
    LK_CMD_GET_STATUS_3 = 0x9D,                   ///< 读取电机状态3

    LK_CMD_TORQUE_OPEN_LOOP = 0xA0,               ///< 转矩开环控制
    LK_CMD_TORQUE_CLOSED_LOOP = 0xA1,             ///< 转矩闭环控制
    LK_CMD_SPEED_CONTROL = 0xA2,                  ///< 速度控制
    LK_CMD_POSITION_CONTROL_MULTI = 0xA4,         ///< 位置控制 (多圈)
    LK_CMD_POSITION_CONTROL_SINGLE = 0xA6,        ///< 位置控制 (单圈)

    LK_CMD_STOP = 0x81,                           ///< 电机停止
    LK_CMD_DISABLE = 0x80,                        ///< 电机关闭
    LK_CMD_ENABLE = 0x88                          ///< 电机运行/开启
} LK_CMD_ID;

/* ==================== 函数声明 ==================== */
/**
 * @brief 多圈角度命令（0x92）
 *
 * 包括命令发送与反馈解析：
 *
 * - LK_GetMultiTurnAngle：发送读取命令
 *   @param CAN_BUS CAN总线句柄
 *   @param CAN_ID 电机ID
 *
 * - LK_MultiTurnAngleFeedback：处理返回的数据帧
 *   @param motor 对应电机对象
 *   @param data CAN帧数据（8字节）
 *   @param len 数据长度
 */
void LK_GetMultiTurnAngle(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID);
void LK_HandleMultiTurnAngleFeedback(MotorDevice *motor, const uint8_t *data, uint8_t len);

/**
 * @brief 单圈角度命令（0x94）
 *
 * 包括命令发送与反馈解析：
 *
 * - LK_GetSingleTurnAngle：发送读取单圈角度命令
 *   @param CAN_BUS CAN总线句柄
 *   @param CAN_ID  电机ID
 *
 * - LK_HandleSingleTurnAngleFeedback：解析单圈角度反馈数据
 *   @param motor 电机对象指针
 *   @param data  CAN帧数据（8字节）
 *   @param len   数据长度
 */
void LK_GetSingleTurnAngle(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID);
void LK_HandleSingleTurnAngleFeedback(MotorDevice *motor, const uint8_t *data, uint8_t len);

/**
 * @brief 状态1 + 错误标志命令（0x9A）
 *
 * - LK_GetStatus1AndError：发送读取状态1与错误标志命令
 *   @param CAN_BUS CAN总线句柄
 *   @param CAN_ID  电机ID
 *
 * - LK_HandleStatus1AndErrorFeedback：解析状态与错误反馈数据
 *   @param motor 电机对象指针
 *   @param data  CAN帧数据（8字节）
 *   @param len   数据长度
 */
void LK_GetStatus1AndError(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID);
void LK_HandleStatus1AndErrorFeedback(MotorDevice *motor, const uint8_t *data, uint8_t len);

/**
 * @brief 清除错误标志命令（0x9B）
 *
 * - LK_ClearErrorFlag：发送清除错误命令
 *   @param CAN_BUS CAN总线句柄
 *   @param CAN_ID  电机ID
 *
 * - LK_HandleClearErrorFlagFeedback：解析清除错误反馈
 *   @param motor 电机对象指针
 *   @param data  CAN帧数据（8字节）
 *   @param len   数据长度
 */
void LK_ClearErrorFlag(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID);
void LK_HandleClearErrorFlagFeedback(MotorDevice *motor, const uint8_t *data, uint8_t len);

/**
 * @brief 状态2命令（0x9C）
 *
 * - LK_GetStatus2：发送读取状态2命令
 *   @param CAN_BUS CAN总线句柄
 *   @param CAN_ID  电机ID
 *
 * - LK_HandleStatus2Feedback：解析状态2反馈
 *   @param motor 电机对象指针
 *   @param data  CAN帧数据（8字节）
 *   @param len   数据长度
 */
void LK_GetStatus2(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID);
void LK_HandleStatus2Feedback(MotorDevice *motor, const uint8_t *data, uint8_t len);

/**
 * @brief 状态3命令（0x9D）
 *
 * - LK_GetStatus3：发送读取状态3命令
 *   @param CAN_BUS CAN总线句柄
 *   @param CAN_ID  电机ID
 *
 * - LK_HandleStatus3Feedback：解析状态3反馈
 *   @param motor 电机对象指针
 *   @param data  CAN帧数据（8字节）
 *   @param len   数据长度
 */
void LK_GetStatus3(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID);
void LK_HandleStatus3Feedback(MotorDevice *motor, const uint8_t *data, uint8_t len);

/**
 * @brief 转矩开环控制命令（0xA0）
 *
 * - LK_TorqueOpenLoop：发送开环转矩命令
 *   @param CAN_BUS     CAN总线句柄
 *   @param CAN_ID      电机ID
 *   @param torqueValue 开环转矩值（单位：0.01Nm）
 *
 * - LK_HandleTorqueOpenLoopFeedback：解析开环转矩控制反馈
 *   @param motor 电机对象指针
 *   @param data  CAN帧数据（8字节）
 *   @param len   数据长度
 */
void LK_TorqueOpenLoop(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID, int16_t torqueValue);
void LK_HandleTorqueOpenLoopFeedback(MotorDevice *motor, const uint8_t *data, uint8_t len);

/**
 * @brief 转矩闭环控制命令（0xA1）
 *
 * - LK_TorqueClosedLoop：发送闭环转矩命令
 *   @param CAN_BUS     CAN总线句柄
 *   @param CAN_ID      电机ID
 *   @param torqueValue 闭环转矩值（单位：0.01Nm）
 *
 * - LK_HandleTorqueClosedLoopFeedback：解析闭环转矩反馈
 *   @param motor 电机对象指针
 *   @param data  CAN帧数据（8字节）
 *   @param len   数据长度
 */
void LK_TorqueClosedLoop(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID, int16_t torqueValue);
void LK_HandleTorqueClosedLoopFeedback(MotorDevice *motor, const uint8_t *data, uint8_t len);

/**
 * @brief 速度控制命令（0xA2）
 *
 * - LK_SpeedControl：发送速度控制命令
 *   @param CAN_BUS       CAN总线句柄
 *   @param CAN_ID        电机ID
 *   @param speedControl  速度值（单位：rpm）
 *
 * - LK_HandleSpeedControlFeedback：解析速度控制反馈
 *   @param motor 电机对象指针
 *   @param data  CAN帧数据（8字节）
 *   @param len   数据长度
 */
void LK_SpeedControl(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID, int32_t speedControl);
void LK_HandleSpeedControlFeedback(MotorDevice *motor, const uint8_t *data, uint8_t len);

/**
 * @brief 多圈位置控制命令（0xA4）
 *
 * - LK_PositionControlMulti：多圈位置控制命令
 *   @param CAN_BUS      CAN总线句柄
 *   @param CAN_ID       电机ID
 *   @param angleControl 位置控制值（单位0.01度/LSB，int32_t）
 *   @param maxSpeed     最大速度限制（单位度/秒LSB，uint16_t）
 *
 * - LK_HandlePositionControlMultiFeedback：解析多圈位置控制反馈
 *   @param motor 电机对象指针
 *   @param data  CAN帧数据（8字节）
 *   @param len   数据长度
 */
void LK_PositionControlMulti(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID, float angleControl, uint16_t maxSpeed);
void LK_HandlePositionControlMultiFeedback(MotorDevice *motor, const uint8_t *data, uint8_t len);

/**
 * @brief 单圈位置控制命令（0xA6）
 *
 * - LK_PositionControlSingle：单圈位置控制命令
 *   @param CAN_BUS       CAN总线句柄
 *   @param CAN_ID        电机ID
 *   @param angleDegrees  目标角度，单位 度，范围 [0, 359.99]
 *   @param spinDirection 旋转方向，0x00顺时针，0x01逆时针
 *   @param maxSpeed      最大速度限制，单位 度/秒
 *
 * - LK_HandlePositionControlSingleFeedback：解析单圈位置控制反馈
 *   @param motor 电机对象指针
 *   @param data  CAN帧数据（8字节）
 *   @param len   数据长度
 */
void LK_PositionControlSingle(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID, float angleControl, uint8_t spinDirection, uint16_t maxSpeed);
void LK_HandlePositionControlSingleFeedback(MotorDevice *motor, const uint8_t *data, uint8_t len);

/**
 * @brief 停止电机命令（0x81）
 *
 * - LK_StopMotor：发送停止电机命令
 *   @param CAN_BUS CAN总线句柄
 *   @param CAN_ID  电机ID
 *
 * - LK_HandleStopMotorFeedback：解析停止反馈
 *   @param motor 电机对象指针
 *   @param data  CAN帧数据
 *   @param len   数据长度
 */
void LK_StopMotor(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID);
void LK_HandleStopMotorFeedback(MotorDevice *motor, const uint8_t *data, uint8_t len);

/**
 * @brief 关闭电机命令（0x80）
 *
 * - LK_DisableMotor：发送关闭电机命令
 *   @param CAN_BUS CAN总线句柄
 *   @param CAN_ID  电机ID
 *
 * - LK_HandleDisableMotorFeedback：解析关闭反馈
 *   @param motor 电机对象指针
 *   @param data  CAN帧数据
 *   @param len   数据长度
 */
void LK_DisableMotor(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID);
void LK_HandleDisableMotorFeedback(MotorDevice *motor, const uint8_t *data, uint8_t len);

/**
 * @brief 使能电机命令（0x88）
 *
 * - LK_EnableMotor：发送使能电机命令
 *   @param CAN_BUS CAN总线句柄
 *   @param CAN_ID  电机ID
 *
 * - LK_HandleEnableMotorFeedback：解析使能反馈
 *   @param motor 电机对象指针
 *   @param data  CAN帧数据
 *   @param len   数据长度
 */
void LK_EnableMotor(CAN_HandleTypeDef *CAN_BUS, uint32_t CAN_ID);
void LK_HandleEnableMotorFeedback(MotorDevice *motor, const uint8_t *data, uint8_t len);

#endif //SAFEPOWER_MOTOR_DRIVE_LK_H
