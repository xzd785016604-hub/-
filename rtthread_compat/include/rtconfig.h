#ifndef __RTCONFIG_H__
#define __RTCONFIG_H__

/* Minimal RT-Thread configuration for STM32F103C8 migration project.
 * Replace rtthread_compat with the official RT-Thread standard source when building the real kernel.
 */
#define RT_NAME_MAX                 8
#define RT_ALIGN_SIZE               4
#define RT_THREAD_PRIORITY_MAX      32
#define RT_TICK_PER_SECOND          1000
#define RT_USING_HEAP
#define RT_USING_SEMAPHORE

#endif
