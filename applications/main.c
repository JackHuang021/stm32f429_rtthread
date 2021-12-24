/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
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
        if (mkdir("/spi", 0x777) == 0)
        {
            dfs_mount("W25Q64", "/spi", "elm", 0, 0);
            rt_kprintf("W25Q64 mount to /spi success\n");
        }
        else
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
