#include "timer_test_app.h"

static rt_timer_t timer1 = RT_NULL;
static rt_timer_t timer2 = RT_NULL;

static int cnt = 0;

static void timer1_timeout(void *parameter)
{
	rt_kprintf("periodic timer is timeout %d\n", cnt);
    if (cnt++ >= 9)
    {
        rt_timer_stop(timer1);
        rt_kprintf("periodic timer is stopped!\n");
    }
}

static void timer2_timeout(void *parameter)
{
    rt_kprintf("one shot timer is timerout\n");
}

void timer_app_init(void)
{
//    rt_system_timer_init();
//    rt_system_timer_thread_init();
    
	timer1 = rt_timer_create("timer1",
                             timer1_timeout,
                             RT_NULL, 10, RT_TIMER_FLAG_PERIODIC);
    if (timer1 != RT_NULL) rt_timer_start(timer1);
    timer2 = rt_timer_create("timer2",
                             timer2_timeout,
                             RT_NULL, 30, RT_TIMER_FLAG_ONE_SHOT);
    if (timer2 != RT_NULL) rt_timer_start(timer2);
}

MSH_CMD_EXPORT(timer_app_init, timer app test);

