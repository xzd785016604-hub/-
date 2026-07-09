#ifndef __COMMON_IMU_H
#define __COMMON_IMU_H
#include "Com_debug.h"
#include "Com_Config.h"
#include "math.h"

/* 表示四元数的结构体 */
typedef struct
{
    float q0;
    float q1;
    float q2;
    float q3;
} Quaternion_Struct;

extern float RtA;
extern float Gyro_G;
extern float Gyro_Gr;

void Common_IMU_GetEulerAngle(Gyro_Accel_Struct *gyroAccel,
                              Euler_struct *eulerAngle,
                              float dt);
float Common_IMU_GetNormAccZ(void);

#endif
