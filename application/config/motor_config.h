#ifndef SAFEPOWER_MOTOR_CONFIG_H
#define SAFEPOWER_MOTOR_CONFIG_H

// ==================== 电机数量与范围定义 ====================
#define WAIST_LEG_MOTOR_SIZE     4
#define WAIST_LEG_START_ID       0x15
#define WAIST_LEG_END_ID         0x18

#define ARM_MOTOR_SIZE           14     // 左右臂各7个
#define LEFT_ARM_START_ID        0x145
#define LEFT_ARM_END_ID          0x14B
#define RIGHT_ARM_START_ID       0x14D
#define RIGHT_ARM_END_ID         0x153

// ==================== EU（腰腿）电机 ID 枚举 ====================
typedef enum {
    EU_WAIST_YAW_ID    = 0x15,
    EU_WAIST_PITCH_ID  = 0x16,
    EU_KNEE_ID         = 0x17,
    EU_ANKLE_ID        = 0x18,
} EU_CAN_ID;

// ==================== LK（手臂）电机 ID 枚举 ====================
// 左臂
typedef enum {
    LK_SHOULDER_INNER_LEFT_ID   = 0x145,
    LK_SHOULDER_OUTER_LEFT_ID   = 0x146,
    LK_UPPER_ARM_LEFT_ID        = 0x147,
    LK_ELBOW_LEFT_ID            = 0x148,
    LK_FOREARM_LEFT_ID          = 0x149,
    LK_WRIST_UPPER_LEFT_ID      = 0x14A,
    LK_WRIST_LOWER_LEFT_ID      = 0x14B,
} LK_LEFT_ARM_ID;

// 右臂
typedef enum {
    LK_SHOULDER_INNER_RIGHT_ID  = 0x14D,
    LK_SHOULDER_OUTER_RIGHT_ID  = 0x14E,
    LK_UPPER_ARM_RIGHT_ID       = 0x14F,
    LK_ELBOW_RIGHT_ID           = 0x150,
    LK_FOREARM_RIGHT_ID         = 0x151,
    LK_WRIST_UPPER_RIGHT_ID     = 0x152,
    LK_WRIST_LOWER_RIGHT_ID     = 0x153,
} LK_RIGHT_ARM_ID;

// ==================== 电机功能名枚举（通用） ====================
typedef enum {
    JOINT_SHOULDER_INNER  = 0,
    JOINT_SHOULDER_OUTER  = 1,
    JOINT_UPPER_ARM       = 2,
    JOINT_ELBOW           = 3,
    JOINT_FOREARM         = 4,
    JOINT_WRIST_UPPER     = 5,
    JOINT_WRIST_LOWER     = 6,
    JOINT_GRIPPER         = 7,   // 预留，未使用
} Motor_Joint_Name;

// ==================== 命令 ID ====================

// LK 系列命令
typedef enum {
    LK_CMD_GET_ANGLE     = 0x92,
    LK_CMD_SPEED_CONTROL = 0xA2,
    LK_CMD_STOP          = 0x81,
} LK_CMD_ID;

// EU 系列命令
typedef enum {
    EU_CMD_GET_ANGLE     = 0x03,
    EU_CMD_SPEED_CONTROL = 0x05,
} EU_CMD_ID;

#endif // SAFEPOWER_MOTOR_CONFIG_H
