/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 * 2023-12-03     Meco Man     support nano version
 */


#include <rtthread.h>

extern void at_nbiot_init(void);
extern int aht10_sensor_example(void);
extern void temp_and_humi_sensor_example(void);

int main()
{
    at_nbiot_init();
    // aht10_sensor_example();
    // temp_and_humi_sensor_example();
    return RT_EOK;
}
