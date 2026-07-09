#include "rtthread.h"
#include "stm32f1xx_hal.h"

/* Board hook kept for RT-Thread standard-port migration. */
void rt_hw_board_init(void)
{
    HAL_Init();
}
