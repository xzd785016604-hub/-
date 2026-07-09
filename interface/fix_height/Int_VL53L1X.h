#ifndef __INT_VL53L1X__
#define __INT_VL53L1X__

#include "vl53l1_platform.h"
#include "VL53L1X_api.h"
#include "VL53L1X_calibration.h"
#include "rtthread.h"
/**
 * @brief 初始化激光测距仪  完成寄存器的配置
 * 
 */
void Int_VL53L1X_Init(void);

/**
 * @brief 读取激光测距仪的距离值
 * 
 */
uint16_t Int_VL53L1X_GetDistance(void);

#endif // __INT_VL53L1X__
