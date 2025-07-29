//
// Created by Steins_Xin on 2025/5/12.
//

#ifndef SAFEPOWER_EUMOTOR_H
#define SAFEPOWER_EUMOTOR_H

#define WAIST_LEG_MOTOR_SIZE 4      // 腰腿电机数量
#define WAIST_LEG_START_ID 0x15     // 腰腿起始电机ID
#define WAIST_LEG_END_ID 0x18       // 腰腿末端电机ID

typedef enum
{
    WAIST_YAW_ID    = 0x15,
    WAIST_PITCH_ID  = 0x16,
    Knee_ID         = 0x17,
    Ankle_DI        = 0x18,
} EU_CAN_ID;

#endif //SAFEPOWER_EUMOTOR_H
