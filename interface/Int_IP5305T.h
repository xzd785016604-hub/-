#ifndef __INT_IP5305T__
#define __INT_IP5305T__

#include "main.h"
#include "rtthread.h"
/**
 * @brief 启动IP5305T电源 避免自动关机
 * 
 */
void Int_IP5305T_start(void);

/**
 * @brief 软件执行关机指令
 * 
 */
void Int_IP5305T_shutdown(void);

#endif // __INT_IP5305T__
