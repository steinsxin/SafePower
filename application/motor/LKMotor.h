//
// Created by Steins_Xin on 2025/3/31.
//

#ifndef SAFEPOWER_LKMOTOR_H
#define SAFEPOWER_LKMOTOR_H

#define ARM_MOTOR_SIZE 8            // 手臂电机数量
#define LEFT_ARM_START_ID 0x145     // 左臂起始电机ID
#define LEFT_ARM_END_ID 0x14C       // 左臂末端电机ID
#define RIGHT_ARM_START_ID 0x14D    // 右臂起始电机ID
#define RIGHT_ARM_END_ID 0x154      // 右臂末端电机ID

typedef enum
{
    /* 左臂电机ID */
    SHOULDER_INNER_LEFT_ID   = 0x145,
    SHOULDER_OUTER_LEFT_ID   = 0x146,
    UPPER_ARM_LEFT_ID        = 0x147,
    ELBOW_LEFT_ID            = 0x148,
    FOREARM_LEFT_ID          = 0x149,
    WRIST_UPPER_LEFT_ID      = 0x14A,
    WRIST_LOWER_LEFT_ID      = 0x14B,
//    GRIPPER_LEFT_ID          = 0x14C,

    /* 右臂电机ID */
    SHOULDER_INNER_RIGHT_ID  = 0x14D,
    SHOULDER_OUTER_RIGHT_ID  = 0x14E,
    UPPER_ARM_RIGHT_ID       = 0x14F,
    ELBOW_RIGHT_ID           = 0x150,
    FOREARM_RIGHT_ID         = 0x151,
    WRIST_UPPER_RIGHT_ID     = 0x152,
    WRIST_LOWER_RIGHT_ID     = 0x153,
//    GRIPPER_RIGHT_ID         = 0x154,
} LK_CAN_ID;

typedef enum
{
    SHOULDER_INNER  = 0x00,
    SHOULDER_OUTER  = 0x01,
    UPPER_ARM       = 0x02,
    ELBOW           = 0x03,
    FOREARM         = 0x04,
    WRIST_UPPER     = 0x05,
    WRIST_LOWER     = 0x06,
    GRIPPER         = 0x07,
} Motor_Name;

#endif //SAFEPOWER_LKMOTOR_H
