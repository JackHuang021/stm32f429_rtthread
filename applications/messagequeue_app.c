
#include "messagequeue_app.h"


static rt_mq_t msgqueue = RT_NULL;
static rt_thread_t thread1 = RT_NULL;
static rt_thread_t thread2 = RT_NULL;

static void thread1_entry(void *parameter)
{
    char buf = 0;
    rt_uint8_t cnt = 0;
    
    while (1)
    {
        if (rt_mq_recv(msgqueue, &buf, sizeof(buf), RT_WAITING_FOREVER) == RT_EOK)
        {
            rt_kprintf("thread1: receive a message from message queue, the content: %c\n", buf);
            if (cnt == 19)
            {
                break;
            }
        }
        cnt ++;
        rt_thread_mdelay(50);
    }
    rt_mq_delete(msgqueue);
    rt_kprintf("thread1: delete message queue.\n");
}

static void thread2_entry(void *parameter)
{
    rt_err_t result = RT_EOK;
    char buf = 'A';
    rt_uint8_t cnt = 0;     
    while (1)
    {
        if (cnt == 8)
        {
            result = rt_mq_urgent(msgqueue, &buf, sizeof(buf));
            if (result != RT_EOK)
            {
                rt_kprintf("thread2: send urgent message failed.\n");
            }
            else
            {
                rt_kprintf("thread2: send urgent message, the content: %c\n", buf);
            }
        }
        else if (cnt >= 20)
        {
            rt_kprintf("thread2: message queue stop send, thread2 leave.\n");
            break;
        }
        else
        {
            result = rt_mq_send(msgqueue, &buf, sizeof(buf));
            if (result != RT_EOK)
            {
                rt_kprintf("thread2: send message failed.\n");
            }
            else
            {
                rt_kprintf("thread2: send a message, the content: %c\n", buf);
            }
        }
        buf ++;
        cnt ++;
        rt_thread_mdelay(5);
    }
}

void messagequeue_app_init(void)
{
    msgqueue = rt_mq_create("msgqueue", 1, 128, RT_IPC_FLAG_PRIO);
    thread1 = rt_thread_create("thread1",
                               thread1_entry, RT_NULL,
                               512, 10, 10);
    rt_thread_startup(thread1);
    
    thread2 = rt_thread_create("thread2",
                               thread2_entry, RT_NULL,
                               512, 10, 10);
    rt_thread_startup(thread2);
}

MSH_CMD_EXPORT(messagequeue_app_init, message queue app test);

