#include "mutex_app.h"

#define THREAD_PRIORITY         8
#define THREAD_TIMESLICE        5

static rt_mutex_t mutex = RT_NULL;
static rt_uint8_t number1 = 0;
static rt_uint8_t number2 = 0;

static rt_thread_t thread1 = RT_NULL;
static rt_thread_t thread2 = RT_NULL;

static void thread1_entry(void *parameter)
{
    while(1)
    {
        rt_mutex_take(mutex, RT_WAITING_FOREVER);
        number1 ++;
        number2 ++;
        rt_mutex_release(mutex);
        rt_thread_delay(10);
    }
}


static void thread2_entry(void *parameter)
{
    while(1)
    {
        rt_mutex_take(mutex, RT_WAITING_FOREVER);
        if (number1 != number2)
            rt_kprintf("not protect, number1 = %d, number2 = %d\n", number1, number2);
        else
            rt_kprintf("mutex protect, number1 = %d, number2 = %d\n", number1, number2);
        number1 ++;
        number2 ++;
        rt_mutex_release(mutex);
        
        if (number1 > 50) return;
    }
}


int mutex_app_init(void)
{
    mutex = rt_mutex_create("mutex", RT_IPC_FLAG_PRIO);
    if (mutex == RT_NULL)
    {
        rt_kprintf("create mutex failed.\n");
        return -1;
    }
    
    thread1 = rt_thread_create("thread1", thread1_entry, 
                               RT_NULL, 512, 
                               THREAD_PRIORITY, THREAD_TIMESLICE);
    if (thread1 != RT_NULL) rt_thread_startup(thread1);
    else rt_kprintf("create thread1 failed.\n");
    
    thread2 = rt_thread_create("thread2", thread2_entry,
                               RT_NULL, 512,
                               THREAD_PRIORITY - 1, THREAD_TIMESLICE);
    if (thread2 != RT_NULL) rt_thread_startup(thread2);
    else rt_kprintf("create thread2 failed.\n");
    
    return 0;
}

MSH_CMD_EXPORT(mutex_app_init, mutex app test);



