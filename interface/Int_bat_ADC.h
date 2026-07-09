#ifndef __INT_BAT_ADC__
#define __INT_BAT_ADC__

#include "adc.h"

/**
 * @brief 启动ADC采样
 * 
 */
void Int_bat_ADC_Init(void);

/**
 * @brief 直接读取电池电压
 * 
 * @return float 
 */
float Int_bat_ADC_Read(void);

#endif // __INT_BAT_ADC__
