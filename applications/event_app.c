#include "event_app.h"


#define EVENT_FLAG_1        (1 << 0)
#define EVENT_FLAG_2        (1 << 1)


static rt_event_t event = RT_NULL;
static rt_thread_t thread1 = RT_NULL;
static rt_thread_t thread2 = RT_NULL;


static void thread1_entry(void *parameter)
{
    rt_uint32_t event_rece;
    
    if (rt_event_recv(event, EVENT_FLAG_1 | EVENT_FLAG_2,
                      RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                      RT_WAITING_FOREVER, &event_rece) == RT_EOK)
    {
        rt_kprintf("thread1: OR event received, event = 0x%x\n", event_rece);
    }
    
    rt_kprintf("thread1: delay 1s to prepare the second event\n");
    rt_thread_delay(1000);
    
    if (rt_event_recv(event, EVENT_FLAG_1 | EVENT_FLAG_2,
                      RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                      RT_WAITING_FOREVER, &event_rece) == RT_EOK)
    {
        rt_kprintf("thread1: AND event received, event = 0x%x\n", event_rece);
    }
    rt_kprintf("thread1 leave.\n");
}

static void thread2_entry(void *parameter)
{
    rt_kprintf("thread2: send event1\n");
    rt_event_send(event, EVENT_FLAG_1);
    rt_thread_delay(200);
    
    rt_kprintf("thread2: send event2\n");
    rt_event_send(event, EVENT_FLAG_2);
    rt_thread_delay(200);
    
    rt_kprintf("thread2: send event3\n");
    rt_event_send(event, EVENT_FLAG_1);
    rt_kprintf("thread2 leave.\n");
}


void event_app_init(void)
{
    event = rt_event_create("event", RT_IPC_FLAG_PRIO);
    
    thread1 = rt_thread_create("thread1", thread1_entry,
                               RT_NULL, 512,
                               8, 10);
    rt_thread_startup(thread1);
    
    thread2 = rt_thread_create("thread2", thread2_entry,
                               RT_NULL, 512,
                               9, 10);
    rt_thread_startup(thread2);
    
}

MSH_CMD_EXPORT(event_app_init, event app test);



