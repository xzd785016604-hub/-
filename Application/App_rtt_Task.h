#ifndef __APP_RTT_TASK_H__
#define __APP_RTT_TASK_H__

#include "rtthread.h"
#include "Com_debug.h"
#include "Com_config.h"
#include "Int_IP5305T.h"
#include "Int_motor.h"
#include "Int_led.h"
#include "Int_SI24R1.h"
#include "Int_bat_ADC.h"
#include "App_receive_data.h"
#include "App_flight.h"

void App_rtthread_start(void);
void App_rtt_notify_power_shutdown(void);
void App_rtt_notify_fail_done(void);
rt_sem_t App_rtt_get_fail_done_sem(void);

#endif /* __APP_RTT_TASK_H__ */
