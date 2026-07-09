#include "App_rtt_Task.h"
#include <stdio.h>

LED_Struct left_top_led = {.port = LED1_GPIO_Port, .pin = LED1_Pin};
LED_Struct right_top_led = {.port = LED2_GPIO_Port, .pin = LED2_Pin};
LED_Struct right_bottom_led = {.port = LED3_GPIO_Port, .pin = LED3_Pin};
LED_Struct left_bottom_led = {.port = LED4_GPIO_Port, .pin = LED4_Pin};

Remote_State remote_state = REMOTE_DISCONNECTED;
Flight_State flight_state = IDLE;
Remote_Data remote_data = {.thr = 0, .yaw = 500, .pit = 500, .rol = 500, .fix_height = 0, .shutdown = 0};
uint16_t fix_height = 0;
uint8_t back_buff[TX_PLOAD_WIDTH] = {0};

static void power_task(void *args);
static void flight_task(void *args);
static void led_task(void *args);
static void com_task(void *args);

/* RT-Thread stack_size unit is byte. Original FreeRTOS stack size was 128 words = 512 bytes. */
#define POWER_TASK_STACK_SIZE       512
#define FLIGHT_TASK_STACK_SIZE      512
#define LED_TASK_STACK_SIZE         512
#define COM_TASK_STACK_SIZE         512

/* RT-Thread priority: smaller number means higher priority. */
#define FLIGHT_TASK_PRIORITY        8
#define COM_TASK_PRIORITY           9
#define POWER_TASK_PRIORITY         12
#define LED_TASK_PRIORITY           20

#define POWER_TASK_PERIOD           10000
#define FLIGHT_TASK_PERIOD          6
#define LED_TASK_PERIOD             100
#define COM_TASK_PERIOD             10

static rt_thread_t power_thread_handle = RT_NULL;
static rt_thread_t flight_thread_handle = RT_NULL;
static rt_thread_t led_thread_handle = RT_NULL;
static rt_thread_t com_thread_handle = RT_NULL;

static rt_sem_t power_sem = RT_NULL;
static rt_sem_t fail_done_sem = RT_NULL;

static void App_rtt_delay_until(rt_tick_t *last_wake_time, rt_tick_t period)
{
    rt_tick_t now = rt_tick_get();
    rt_tick_t next = *last_wake_time + period;

    if ((rt_int32_t)(next - now) > 0)
    {
        rt_thread_delay(next - now);
    }

    *last_wake_time = next;
}

void App_rtt_notify_power_shutdown(void)
{
    if (power_sem != RT_NULL)
    {
        rt_sem_release(power_sem);
    }
}

void App_rtt_notify_fail_done(void)
{
    if (fail_done_sem != RT_NULL)
    {
        rt_sem_release(fail_done_sem);
    }
}

rt_sem_t App_rtt_get_fail_done_sem(void)
{
    return fail_done_sem;
}

void App_rtthread_start(void)
{
    power_sem = rt_sem_create("psem", 0, RT_IPC_FLAG_FIFO);
    fail_done_sem = rt_sem_create("fsem", 0, RT_IPC_FLAG_FIFO);

    power_thread_handle = rt_thread_create("power", power_task, RT_NULL,
                                           POWER_TASK_STACK_SIZE,
                                           POWER_TASK_PRIORITY, 10);
    if (power_thread_handle != RT_NULL)
    {
        rt_thread_startup(power_thread_handle);
    }

    flight_thread_handle = rt_thread_create("flight", flight_task, RT_NULL,
                                            FLIGHT_TASK_STACK_SIZE,
                                            FLIGHT_TASK_PRIORITY, 10);
    if (flight_thread_handle != RT_NULL)
    {
        rt_thread_startup(flight_thread_handle);
    }

    led_thread_handle = rt_thread_create("led", led_task, RT_NULL,
                                         LED_TASK_STACK_SIZE,
                                         LED_TASK_PRIORITY, 10);
    if (led_thread_handle != RT_NULL)
    {
        rt_thread_startup(led_thread_handle);
    }

    com_thread_handle = rt_thread_create("com", com_task, RT_NULL,
                                         COM_TASK_STACK_SIZE,
                                         COM_TASK_PRIORITY, 10);
    if (com_thread_handle != RT_NULL)
    {
        rt_thread_startup(com_thread_handle);
    }

    rt_system_scheduler_start();
}

static void power_task(void *args)
{
    (void)args;

    while (1)
    {
        rt_err_t res = rt_sem_take(power_sem, POWER_TASK_PERIOD);
        if (res == RT_EOK)
        {
            Int_IP5305T_shutdown();
        }
        else
        {
            Int_IP5305T_start();
        }
    }
}

static void flight_task(void *args)
{
    rt_tick_t xLastWakeTime = rt_tick_get();
    uint8_t count = 0;
    (void)args;

    App_flight_init();

    while (1)
    {
        App_flight_get_euler_angle();
        App_flight_pid_process();

        if (flight_state == FIX_HEIGHT)
        {
            count++;
            if (count >= 4)
            {
                App_flight_fix_height_pid_process();
                count = 0;
            }
        }

        App_flight_control_motor();
        App_rtt_delay_until(&xLastWakeTime, FLIGHT_TASK_PERIOD);
    }
}

static void led_task(void *args)
{
    rt_tick_t xLastWakeTime = rt_tick_get();
    uint8_t count = 0;
    (void)args;

    while (1)
    {
        count++;

        if (remote_state == REMOTE_CONNECTED)
        {
            Int_led_turn_on(&left_top_led);
            Int_led_turn_on(&right_top_led);
        }
        else if (remote_state == REMOTE_DISCONNECTED)
        {
            Int_led_turn_off(&left_top_led);
            Int_led_turn_off(&right_top_led);
        }

        if (flight_state == IDLE)
        {
            if (count % 5 == 0)
            {
                Int_led_toggle(&left_bottom_led);
                Int_led_toggle(&right_bottom_led);
            }
        }
        else if (flight_state == NORMAL)
        {
            if (count % 2 == 0)
            {
                Int_led_toggle(&left_bottom_led);
                Int_led_toggle(&right_bottom_led);
            }
        }
        else if (flight_state == FIX_HEIGHT)
        {
            Int_led_turn_on(&left_bottom_led);
            Int_led_turn_on(&right_bottom_led);
        }
        else if (flight_state == FAIL)
        {
            Int_led_turn_off(&left_bottom_led);
            Int_led_turn_off(&right_bottom_led);
        }

        if (count == 10)
        {
            count = 0;
        }

        App_rtt_delay_until(&xLastWakeTime, LED_TASK_PERIOD);
    }
}

static void com_task(void *args)
{
    (void)args;

    Int_bat_ADC_Init();

    while (1)
    {
        uint8_t res = App_receive_data();

        App_process_connect_state(res);

        if (remote_data.shutdown == 1)
        {
            App_rtt_notify_power_shutdown();
        }

        App_process_flight_state();

        {
            float voltage = Int_bat_ADC_Read();
            sprintf((char *)back_buff, "%.2f", voltage);
        }

        rt_thread_mdelay(COM_TASK_PERIOD);
    }
}
