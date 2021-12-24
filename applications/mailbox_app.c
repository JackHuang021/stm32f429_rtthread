#include "mailbox_app.h"

static rt_mailbox_t mailbox = RT_NULL;
static rt_thread_t thread1 = RT_NULL;
static rt_thread_t thread2 = RT_NULL;

static char mb_str1[] = "I'm a mail.";
static char mb_str2[] = "This is another mail.";
static char mb_str3[] = "over.";


static void thread1_entry(void* parameter)
{
    char *str = RT_NULL;
    while (1)
    {
        rt_kprintf("thread1: try to receive a mail\n");
        if (rt_mb_recv(mailbox, (rt_ubase_t *)&str, RT_WAITING_FOREVER) == RT_EOK)
        {
            rt_kprintf("thread1: get a mail from mailbox, the content: %s\n", str);
            if (str == mb_str3)
            {
                break;
            }
            rt_thread_delay(100);
        }
    }
    rt_mb_delete(mailbox);
}

static void thread2_entry(void* parameter)
{
    rt_uint8_t cnt = 0;
    while (cnt < 10)
    {
        cnt ++;
        if (cnt & 0x01)
        {
            rt_mb_send(mailbox, (rt_ubase_t)&mb_str1);
        }
        else
        {
            rt_mb_send(mailbox, (rt_ubase_t)&mb_str2);
        }
        rt_thread_delay(200);
    }
    rt_mb_send(mailbox, (rt_ubase_t)&mb_str3);
}

void mailbox_app_init(void)
{
    mailbox = rt_mb_create("mailbox", 3, RT_IPC_FLAG_PRIO);
    
    thread1 = rt_thread_create("thread1", 
                               thread1_entry, RT_NULL,
                               512, 10, 10);
    rt_thread_startup(thread1);
    
    thread2 = rt_thread_create("thread2",
                               thread2_entry, RT_NULL,
                               512, 10, 10);
    rt_thread_startup(thread2);
}

MSH_CMD_EXPORT(mailbox_app_init, mailbox app test);


