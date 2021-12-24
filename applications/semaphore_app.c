#include "semaphore_app.h"

static rt_sem_t sem = RT_NULL;

#define THREAD_PRIORITY         25
#define THREAD_TIMESLICE        5

static rt_thread_t thread1 = RT_NULL;
static rt_thread_t thread2 = RT_NULL;


static void thread1_entry(void *parameter)
{
    static rt_uint8_t cnt = 0;
    while (1)
    {
        if (cnt <= 100)
        {
            cnt ++;
        }
        else return;
        
        if (0 == (cnt % 10))
        {
            rt_kprintf("thread1 release a dynamic semaphore.\n");
            rt_sem_release(sem);
        }
    }
}

static void thread2_entry(void *parameter)
{
    static rt_err_t result;
    static rt_uint8_t number = 0;
    
    while(1)
    {
        result = rt_sem_take(sem, RT_WAITING_FOREVER);
        if (result != RT_EOK)
        {
            rt_kprintf("thread2 take a semaphore failed.\n");
            rt_sem_delete(sem);
            return;
        }
        else
        {
            number ++;
            rt_kprintf("thread2 take a semaphore, number = %d\n", number);
        }
    }

}

int semaphore_app_init(void)
{
    sem = rt_sem_create("sem", 0, RT_IPC_FLAG_PRIO);
    if (sem == RT_NULL)
    {
        rt_kprintf("creat semaphore failed...\n");
        return -1;
    }
    else
    {
        rt_kprintf("creat done. dynamic semaphore value = 0.\n");
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

MSH_CMD_EXPORT(semaphore_app_init, semaphore test app);



