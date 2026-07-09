#ifndef __RTTHREAD_H__
#define __RTTHREAD_H__

#include <stdint.h>
#include <stddef.h>
#include "rtconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RT_NULL                     ((void *)0)
#define RT_EOK                      0
#define RT_ERROR                    1
#define RT_ETIMEOUT                 2
#define RT_WAITING_FOREVER          ((rt_int32_t)-1)
#define RT_IPC_FLAG_FIFO            0x00
#define RT_TRUE                     1
#define RT_FALSE                    0

typedef int                         rt_err_t;
typedef int8_t                      rt_int8_t;
typedef uint8_t                     rt_uint8_t;
typedef int16_t                     rt_int16_t;
typedef uint16_t                    rt_uint16_t;
typedef int32_t                     rt_int32_t;
typedef uint32_t                    rt_uint32_t;
typedef uint32_t                    rt_tick_t;
typedef uint8_t                     rt_bool_t;
typedef void (*rt_thread_entry_t)(void *parameter);

struct rt_thread;
typedef struct rt_thread *rt_thread_t;
struct rt_semaphore;
typedef struct rt_semaphore *rt_sem_t;

rt_tick_t rt_tick_get(void);
rt_tick_t rt_tick_from_millisecond(rt_int32_t ms);
void rt_tick_increase(void);
void rt_thread_delay(rt_tick_t tick);
void rt_thread_mdelay(rt_int32_t ms);

rt_thread_t rt_thread_create(const char *name,
                             rt_thread_entry_t entry,
                             void *parameter,
                             rt_uint32_t stack_size,
                             rt_uint8_t priority,
                             rt_uint32_t tick);
rt_err_t rt_thread_startup(rt_thread_t thread);

rt_sem_t rt_sem_create(const char *name, rt_uint32_t value, rt_uint8_t flag);
rt_err_t rt_sem_take(rt_sem_t sem, rt_int32_t time);
rt_err_t rt_sem_release(rt_sem_t sem);

void rt_interrupt_enter(void);
void rt_interrupt_leave(void);
void rt_system_scheduler_start(void);
int rtthread_startup(void);

#ifndef rt_kprintf
#define rt_kprintf(...)
#endif

#ifdef __cplusplus
}
#endif

#endif /* __RTTHREAD_H__ */
