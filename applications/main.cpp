/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-05-23     tyustli   first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <dfs_fs.h>

/* defined the LED0 pin: PB1 */
#define LED0_PIN    GET_PIN(G, 7)

int main(void)
{
    /* mount file system */
    while (1)
    {
        rt_device_t dev_sd;
        rt_device_t dev_spi;
        DIR *dir;
        dev_sd = rt_device_find("sd0");
        dev_spi = rt_device_find("W25Q64");
            
        if (dev_sd == RT_NULL || dev_spi == RT_NULL)
        {
            rt_thread_delay(100);
            continue;
        }

        dfs_mount("sd0", "/", "elm", 0, 0);
        rt_kprintf("sd0 mount to / success.\n");
//        if (mkdir("/spi", 0x777) == 0)
//        {
//            dfs_mount("W25Q64", "/spi", "elm", 0, 0);
//            rt_kprintf("W25Q64 mount to /spi success\n");
//        }
//        else
        {
            dir = opendir("/spi");
            if (dir != RT_NULL) closedir(dir);
            else break;
            dfs_mount("W25Q64", "/spi", "elm", 0, 0);
            rt_kprintf("W25Q64 mount to /spi success\n");
        }
		break;
    }

    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);

    while (1)
    {
        rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED0_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }
}
using namespace rtthread;

class tran
{
public:
    void getnumber(int a, int b)
    {
        x = a;
        y = b;
    }
    void out(tran & s)
    {
        rt_kprintf("x = %d, y = %d\n", x, y);
    }
private:
    int x, y;
};

int test_cpp(void)
{
    tran s;

    s.getnumber(13, 54);
    s.out(s);

    return 0;
}

MSH_CMD_EXPORT(test_cpp, test cpp);

//#include <math.h>
//#include <stdlib.h>

//#define MIN_VALUE                 (1e-4)
//#define IS_DOUBLE_ZERO(d)         (abs(d) < MIN_VALUE)

//double div_func(double x, double y)
//{
//    if (IS_DOUBLE_ZERO(y))
//    {
//        throw y;                                           /* throw exception */
//    }

//    return x / y;
//}

//void throw_exceptions(void *args)
//{
//    try
//    {
//        div_func(6, 3);
//        rt_kprintf("there is no err\n");
//        div_func(4, 0);                                   /* create exception*/
//        rt_kprintf("you can run here\n");
//    }
//    catch(double)                                         /* catch exception */
//    {
//        rt_kprintf("error of dividing zero\n");
//    }
//}

//MSH_CMD_EXPORT(throw_exceptions, throw cpp exceptions);


