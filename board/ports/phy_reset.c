/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-21     SummerGift   add port file
 */

#include <rtthread.h>
#include <board.h>



#define RESET_IO    GET_PIN(E, 3)

rt_err_t phy_init(void)
{
    rt_pin_mode(RESET_IO, PIN_MODE_OUTPUT);
    rt_pin_write(RESET_IO, PIN_HIGH);
    return RT_EOK;
}

/* phy reset */
void phy_reset(void)
{
    rt_pin_write(RESET_IO, PIN_LOW);
    rt_thread_mdelay(100);
    rt_pin_write(RESET_IO, PIN_HIGH);
    rt_thread_mdelay(100);
}

