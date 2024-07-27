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

#include <board.h>
#include <rtthread.h>
#include <drv_gpio.h>
#include <rtdevice.h>
#include <at.h>
#include <finsh.h>
#include "aht10.h"

#define DBG_TAG "AT"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>


#define AT_DIVICE_NAME "uart2"
#define AT_SRING_MAX_SIZE 512
#define AT_WAIT_RESP_MAX_MILLISECOND 300

rt_err_t atcmd_qlwconfig(
    at_response_t resp, 
    int server_type, 
    const char *serverIp, 
    int port, 
    const char *endpoint_name,
    int lifetime,
    int security_mode,
    const char *PSK_ID,
    const char *PSK
) 
{
    // AT+QLWCONFIG=0,"220.180.239.212",8007,"866971030000710",86400,0,"866971030000710","77c755bd8cbb400abf2f0c420eed2cb0"
    char at_string[AT_SRING_MAX_SIZE] = {0};
    rt_snprintf(
        at_string, 
        512, 
        "AT+QLWCONFIG=%d,\"%s\",%d,\"%s\",%d,%d,\"%s\",\"%s\"",
        server_type,
        serverIp,
        port,
        endpoint_name,
        lifetime,
        security_mode,
        PSK_ID,
        PSK,
        "\r"
    );
    return at_exec_cmd(resp, at_string);
}


rt_err_t atcmd_qlwreg(at_response_t resp)
{
    return at_exec_cmd(resp, "AT+QLWREG\r");
}


rt_err_t atcmd_qlwupdate()
{

}


int at_Client_send(int argc, char**argv)
{
    rt_err_t err = RT_EOK;

    if (argc != 2) {
        LOG_E("at_cli_send [command]  - AT client send commands to AT server.\n");
        return -RT_ERROR;
    }

    at_response_t resp = RT_NULL;
    /* 创建响应结构体，设置最大支持响应数据长度为 512 字节，响应数据行数无限制，超时时间为 300 ms */
    resp = at_create_resp(512, 0, rt_tick_from_millisecond(AT_WAIT_RESP_MAX_MILLISECOND));
    if (!resp) {
        LOG_E("No memory for response structure!\n");
        return -RT_ENOMEM;
    }

    // AT+QLWCONFIG=0,"220.180.239.212",8007,"866971030000710",86400,0,"866971030000710","77c755bd8cbb400abf2f0c420eed2cb0"
    const char *cmd = argv[1];
    if (!rt_strcmp(cmd, "atcmd_qlwconfig")) {
        err = atcmd_qlwconfig(
            resp,
            0,
            "220.180.239.212",
            8007,
            "866971030000710",
            86400,
            0,
            "866971030000710",
            "77c755bd8cbb400abf2f0c420eed2cb0"
        );
    }
    else if (!rt_strcmp(cmd, "atcmd_qlwreg")) {
        err = atcmd_qlwreg(resp);
    }
    else {
        LOG_E("atcmd_send unknown cmd: %s\n", cmd);
        err = RT_ERROR;
    }

    LOG_D("atcmd_send err: %d\n", err);
    char *line_ptr = resp->buf;
    for (int i=1; i <= resp->line_counts; i++) {
        LOG_D("recv line %d: %s", i, line_ptr);
        line_ptr += rt_strlen(line_ptr) + 1;
    }

    /* 删除响应结构体 */
    at_delete_resp(resp);

    return err;
}


/* 输出 at_Client_send 函数到 msh 中 */
MSH_CMD_EXPORT(at_Client_send, AT Client send commands to AT Server and get response data);


// -------------AT URT---------------

static void urc_conn_func(struct at_client *client ,const char *data, rt_size_t size)
{
    /* WIFI 连接成功信息 */
    LOG_D("AT Server device WIFI connect success!");
}


static void urc_recv_func(struct at_client *client ,const char *data, rt_size_t size)
{
    /* 接收到服务器发送数据 */
    LOG_D("AT Client receive AT Server data: %s, size: %d\n", data, size);
}


static void urc_func(struct at_client *client ,const char *data, rt_size_t size)
{
    /* 设备启动信息 */
    LOG_D("AT Server device startup!");
}


static struct at_urc urc_table[] = {
    {"WIFI CONNECTED",  "\r\n",     urc_conn_func},
    {"+RECV:",           "\r\n",        urc_recv_func},
    {"RDY",             "\r\n",     urc_func},
};


void at_example(void)
{
    int err = 0;

    // 初始化at client
    err = at_client_init(AT_DIVICE_NAME,  512, 512);
    if (err != 0) {
        LOG_D("at_client_init err: %d\n", err);
    }

    /* 添加多种 URC 数据至 URC 列表中，当接收到同时匹配 URC 前缀和后缀的数据，执行 URC 函数  */
    at_set_urc_table(urc_table, sizeof(urc_table) / sizeof(urc_table[0]));
}


int aht10_sensor_example(void)
{
    float humidity, temperature;
    aht10_device_t dev;

    /* 总线名称 */
    const char *i2c_bus_name = "i2c4";
    int count = 0;

    /* 等待传感器正常工作 */
    rt_thread_mdelay(2000);

    /* 初始化 aht10 */
    dev = aht10_init(i2c_bus_name);
    if (dev == RT_NULL)
    {
        LOG_E(" The sensor initializes failure");
        return 0;
    }

    while (count++ < 100)
    {
        /* 读取湿度 */
        humidity = aht10_read_humidity(dev);
        LOG_D("humidity   : %d.%d %%", (int)humidity, (int)(humidity * 10) % 10);

        /* 读取温度 */
        temperature = aht10_read_temperature(dev);
        LOG_D("temperature: %d.%d", (int)temperature, (int)(temperature * 10) % 10);

        rt_thread_mdelay(1000);
    }
    return 0;
}


int main()
{
    aht10_sensor_example();
    return RT_EOK;
}
