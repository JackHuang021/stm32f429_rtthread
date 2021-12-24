#include "mutex_app2.h"



static rt_mutex_t mutex = RT_NULL;
static rt_thread_t thread1 = RT_NULL;
static rt_thread_t thread2 = RT_NULL;
static rt_thread_t thread3 = RT_NULL;

#define THREAD_PRIORITY     10
#define THREAD_STACK_SIZE   512
#define THREAD_TIMESLCE     5

static void thread1_entry(void *parameter)
{
    rt_thread_delay(100);
    if (thread2->current_priority != thread3->current_priority)
    {
        rt_kprintf("the priority of thread2 is: %d\n", thread2->current_priority);
        rt_kprintf("the priority of thread3 is: %d\n", thread3->current_priority);
        rt_kprintf("test failed.\n");
        return;
    }
    else
    {
        rt_kprintf("the priority of thread2 is: %d\n", thread2->current_priority);
        rt_kprintf("the priority of thread3 is: %d\n", thread3->current_priority);
        rt_kprintf("test OK.\n");    
    }
}

static void thread2_entry(void *parameter)
{
    rt_err_t result;
    rt_kprintf("the thread2 priority is: %d\n", thread2->current_priority);
    rt_thread_delay(50);
    result = rt_mutex_take(mutex, RT_WAITING_FOREVER);
    if (result == RT_EOK)
    {
        rt_mutex_release(mutex);
    }
}

static void thread3_entry(void *parameter)
{
    rt_tick_t tick;
    rt_err_t result;
    
    rt_kprintf("the thread3 priority is: %d\n", thread3->current_priority);
    result = rt_mutex_take(mutex, RT_WAITING_FOREVER);
    if (result != RT_EOK)
    {
        rt_kprintf("thread3 take a mutex failed.\n");
        return;
    }
    else
    tick = rt_tick_get();
    while(rt_tick_get() - tick < (RT_TICK_PER_SECOND / 2));
    rt_mutex_release(mutex);
}


void mutex_app2_init(void)
{
    mutex = rt_mutex_create("mutex", RT_IPC_FLAG_PRIO);
    thread1 = rt_thread_create("thread1", thread1_entry,
                               RT_NULL, THREAD_STACK_SIZE,
                               THREAD_PRIORITY - 1, THREAD_TIMESLCE);
    rt_thread_startup(thread1);
    
    thread2 = rt_thread_create("thread2", thread2_entry,
                               RT_NULL, THREAD_STACK_SIZE,
                               THREAD_PRIORITY, THREAD_TIMESLCE);
    rt_thread_startup(thread2);
    
    thread3 = rt_thread_create("thread3", thread3_entry,
                               RT_NULL, THREAD_STACK_SIZE,
                               THREAD_PRIORITY + 1, THREAD_TIMESLCE);
    rt_thread_startup(thread3);
}

MSH_CMD_EXPORT(mutex_app2_init, mutex2 app test);




