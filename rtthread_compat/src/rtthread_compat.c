#include "rtthread.h"

/*
 * This file is a tiny RT-Thread API compatibility layer so the migrated MDK
 * project can be opened and compiled without the full RT-Thread source tree.
 * It is NOT a replacement for the real RT-Thread standard kernel scheduler.
 * For real flight tests, replace this folder with official RT-Thread source:
 *   include/, src/, libcpu/arm/cortex-m3/
 */

#define RTT_COMPAT_MAX_THREADS      8
#define RTT_COMPAT_MAX_SEMAPHORES   8

struct rt_thread
{
    const char *name;
    rt_thread_entry_t entry;
    void *parameter;
    rt_uint32_t stack_size;
    rt_uint8_t priority;
    rt_uint32_t tick;
    rt_uint8_t started;
};

struct rt_semaphore
{
    const char *name;
    volatile rt_uint32_t value;
};

static volatile rt_tick_t g_rt_tick = 0;
static struct rt_thread g_threads[RTT_COMPAT_MAX_THREADS];
static rt_uint8_t g_thread_count = 0;
static struct rt_semaphore g_sems[RTT_COMPAT_MAX_SEMAPHORES];
static rt_uint8_t g_sem_count = 0;

rt_tick_t rt_tick_get(void)
{
    return g_rt_tick;
}

rt_tick_t rt_tick_from_millisecond(rt_int32_t ms)
{
    if (ms <= 0)
    {
        return 0;
    }
    return (rt_tick_t)(((rt_uint32_t)ms * RT_TICK_PER_SECOND + 999) / 1000);
}

void rt_tick_increase(void)
{
    g_rt_tick++;
}

void rt_thread_delay(rt_tick_t tick)
{
    rt_tick_t start = rt_tick_get();
    while ((rt_tick_t)(rt_tick_get() - start) < tick)
    {
        /* wait for SysTick */
    }
}

void rt_thread_mdelay(rt_int32_t ms)
{
    rt_thread_delay(rt_tick_from_millisecond(ms));
}

rt_thread_t rt_thread_create(const char *name,
                             rt_thread_entry_t entry,
                             void *parameter,
                             rt_uint32_t stack_size,
                             rt_uint8_t priority,
                             rt_uint32_t tick)
{
    struct rt_thread *thread;

    if (g_thread_count >= RTT_COMPAT_MAX_THREADS || entry == RT_NULL)
    {
        return RT_NULL;
    }

    thread = &g_threads[g_thread_count++];
    thread->name = name;
    thread->entry = entry;
    thread->parameter = parameter;
    thread->stack_size = stack_size;
    thread->priority = priority;
    thread->tick = tick;
    thread->started = 0;
    return thread;
}

rt_err_t rt_thread_startup(rt_thread_t thread)
{
    if (thread == RT_NULL)
    {
        return -RT_ERROR;
    }
    thread->started = 1;
    return RT_EOK;
}

rt_sem_t rt_sem_create(const char *name, rt_uint32_t value, rt_uint8_t flag)
{
    struct rt_semaphore *sem;
    (void)flag;

    if (g_sem_count >= RTT_COMPAT_MAX_SEMAPHORES)
    {
        return RT_NULL;
    }

    sem = &g_sems[g_sem_count++];
    sem->name = name;
    sem->value = value;
    return sem;
}

rt_err_t rt_sem_take(rt_sem_t sem, rt_int32_t time)
{
    rt_tick_t start;

    if (sem == RT_NULL)
    {
        return -RT_ERROR;
    }

    start = rt_tick_get();
    while (sem->value == 0)
    {
        if (time != RT_WAITING_FOREVER)
        {
            if ((rt_int32_t)(rt_tick_get() - start) >= time)
            {
                return -RT_ETIMEOUT;
            }
        }
    }

    sem->value--;
    return RT_EOK;
}

rt_err_t rt_sem_release(rt_sem_t sem)
{
    if (sem == RT_NULL)
    {
        return -RT_ERROR;
    }

    sem->value++;
    return RT_EOK;
}

void rt_interrupt_enter(void)
{
}

void rt_interrupt_leave(void)
{
}

void rt_system_scheduler_start(void)
{
    /* Stub only. Official RT-Thread scheduler starts here in the real kernel. */
}

int rtthread_startup(void)
{
    return 0;
}
