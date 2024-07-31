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
#include <drv_gpio.h>
#include <rtdevice.h>
#include <at.h>
#include <finsh.h>
#include  <string.h>

#define DBG_TAG "AT"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>


#define AT_NBIOT_DIVICE_NAME "uart2"
#define AT_STRING_MAX_SIZE 512
#define AT_WAIT_RESP_MAX_MILLISECOND 3000


rt_err_t atcmd_send(const char *dev_name, at_response_t resp, const char *at_string)
{   
    at_client_t client = at_client_get(dev_name);
    if (client == RT_NULL) {
        LOG_E("at_client_init failed.\n");
        return RT_ERROR;
    }
    return at_obj_exec_cmd(client, resp, at_string);
}


rt_err_t atcmd_qlwconfig(
    const char *dev_name,
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
    // AT+QLWCONFIG=<server_type>,<serverIP>,<port>,<endpoint_name>,<lifetime>,<security_mode>[,<PSK_ID>,<PSK>]
    char at_string[AT_STRING_MAX_SIZE] = {0};
    rt_snprintf(
        at_string, 
        AT_STRING_MAX_SIZE, 
        "AT+QLWCONFIG=%d,\"%s\",%d,\"%s\",%d,%d",
        server_type,
        serverIp,
        port,
        endpoint_name,
        lifetime,
        security_mode
    );
    if (PSK_ID != RT_NULL) {
        strcat(at_string, ",\"");
        strcat(at_string, PSK_ID);
        strcat(at_string, "\"");
    }
    if (PSK != RT_NULL) {
        strcat(at_string, ",\"");
        strcat(at_string, PSK);
        strcat(at_string, "\"");
    }
    strcat(at_string, "\r");
    return atcmd_send(dev_name, resp, at_string);
}


rt_err_t atcmd_qlwreg(const char *dev_name, at_response_t resp)
{ 
    // AT+QLWREG
    return atcmd_send(dev_name, resp, "AT+QLWREG\r");
}


rt_err_t atcmd_qlwupdate(
    const char *dev_name, 
    at_response_t resp,
    int mode,
    int lifetime_or_binding_mode,
    int server_type
)
{
    // AT+QLWUPDATE=<mode>,<lifetime/binding_mode>[,<server_type>]
    char at_string[AT_STRING_MAX_SIZE] = {0};
    if (server_type == 0 || server_type == 2 || server_type == 3) {
        rt_snprintf(
            at_string,
            AT_STRING_MAX_SIZE,
            "AT+QLWUPDATE=%d,%d,%d%c",
            mode,
            lifetime_or_binding_mode,
            server_type,
            '\r'
        );
    }
    else {
        rt_snprintf(
            at_string,
            AT_STRING_MAX_SIZE,
            "AT+QLWUPDATE=%d,%d,%c",
            mode,
            lifetime_or_binding_mode,
            '\r'
        );
    }

    return atcmd_send(dev_name, resp, at_string);
}


rt_err_t atcmd_qlwdereg(
    const char *dev_name, 
    at_response_t resp
)
{
    // AT+QLWDEREG
    return atcmd_send(dev_name, resp, "AT+QLWDEREG\r");
}


rt_err_t atcmd_qlwaddobj(const char *dev_name, at_response_t resp, int objectID, int instanceID, int res_num, int *resourceIDs)
{
    // AT+QLWADDOBJ=<objectID>,<instanceID>,<res_num>,<resourceID>
    char temp[20] = {0};
    char at_string[AT_STRING_MAX_SIZE] = {0};
    rt_snprintf(
        at_string,
        AT_STRING_MAX_SIZE,
        "AT+QLWADDOBJ=%d,%d,%d",
        objectID,
        instanceID,
        res_num
    );
    for (int i=0; i < res_num; i++) {
        rt_memset(temp, 0, 20);
        rt_snprintf(temp, 20, ",%d", resourceIDs[i]);
        strcat(at_string, temp);
    }
    strcat(at_string, "\r");
    return atcmd_send(dev_name, resp, at_string);
}


rt_err_t atcmd_qlwdelobj(const char *dev_name, at_response_t resp, int objectID)
{
    // AT+QLWDELOBJ=<objectID>
    char at_string[AT_STRING_MAX_SIZE] = {0};
    rt_snprintf(
        at_string,
        AT_STRING_MAX_SIZE,
        "AT+QLWDELOBJ=%d\r",
        objectID
    );
    return atcmd_send(dev_name, resp, at_string);
}


rt_err_t atcmd_qlwrdrsp(
    const char *dev_name, 
    at_response_t resp, 
    int messageID, 
    int result, 
    int objectID, 
    int instanceID,
    int resourceID,
    int value_type,
    int len,
    const char *value,
    int index
)
{
    // AT+QLWRDRSP=<messageID>,<result>,<objectID>,<instanceID>,<resourceID>,<value_type>,<len>,<value>,<index>
    char at_string[AT_STRING_MAX_SIZE] = {0};
    rt_snprintf(
        at_string,
        AT_STRING_MAX_SIZE,
        "AT+QLWRDRSP=%d,%d,%d,%d,%d,%d,%d,\"%s\",%d\r",
        messageID,
        result,
        objectID,
        instanceID,
        resourceID,
        value_type,
        len,
        value,
        index
    );
    return atcmd_send(dev_name, resp, at_string);
}


rt_err_t atcmd_qlwwrrsp(
    const char *dev_name, 
    at_response_t resp, 
    int messageID, 
    int result
)
{
    // AT+QLWWRRSP=<messageID>,<result>
    char at_string[AT_STRING_MAX_SIZE] = {0};
    rt_snprintf(
        at_string,
        AT_STRING_MAX_SIZE,
        "AT+QLWRDRSP=%d,%d\r",
        messageID,
        result
    );
    return atcmd_send(dev_name, resp, at_string);
}


rt_err_t atcmd_qlwexeresp(
    const char *dev_name, 
    at_response_t resp, 
    int messageID,
    int result
)
{
    // AT+QLWEXERSP=<messageID>,<result>
    char at_string[AT_STRING_MAX_SIZE] = {0};
    rt_snprintf(
        at_string,
        AT_STRING_MAX_SIZE,
        "AT+QLWEXERSP=%d,%d\r",
        messageID,
        result
    );
    return atcmd_send(dev_name, resp, at_string);
}


rt_err_t atcmd_qlwobsrsp(
    const char *dev_name, 
    at_response_t resp, 
    int messageID,
    int result,
    int objectID,
    int instanceID,
    int resourceID,
    int value_type,
    int len,
    const char *value,
    int index
)
{
    // AT+QLWOBSRSP=<messageID>,<result>,<objectID>,<instanceID>,<resourceID>,<value_type>,<len>,<value>,<index>
    char at_string[AT_STRING_MAX_SIZE] = {0};
    rt_snprintf(
        at_string,
        AT_STRING_MAX_SIZE,
        "AT+QLWOBSRSP=%d,%d,%d,%d,%d,%d,%d,\"%s\",%d\r",
        messageID,
        result,
        objectID,
        instanceID,
        resourceID,
        value_type,
        len,
        value,
        index
    );
    return atcmd_send(dev_name, resp, at_string);
}


rt_err_t atcmd_qlwnotify(
    const char *dev_name, 
    at_response_t resp,
    int objectID,
    int instanceID,
    int resourceID,
    int value_type,
    int len,
    const char *value,
    int index,
    int ack,
    int rai_flag
)
{
    // AT+QLWNOTIFY=<objectID>,<instanceID>,<resourceID>,<value_type>,<len>,<value>,<index>[,<ack>[,<rai_flag>]]
    char temp[20] = {0};
    char at_string[AT_STRING_MAX_SIZE] = {0};
    rt_snprintf(
        at_string,
        AT_STRING_MAX_SIZE,
        "AT+QLWNOTIFY=%d,%d,%d,%d,%d,%s,%d",
        objectID,
        instanceID,
        resourceID,
        value_type,
        len,
        value,
        index
    );
    if (ack == 0 || ack == 1) {
        rt_memset(temp, 0, 20);
        rt_snprintf(temp, 20, ",%d", ack);
        strcat(at_string, temp);
    }
    if (rai_flag == 0 || rai_flag == 1 || rai_flag == 2) {
        rt_memset(temp, 0, 20);
        rt_snprintf(temp, 20, ",%d", rai_flag);
        strcat(at_string, temp);
    }
    strcat(at_string, "\r");
    return atcmd_send(dev_name, resp, at_string);
}


rt_err_t atcmd_qlwstatus(const char *dev_name, at_response_t resp)
{
    // AT+QLWSTATUS?
    return atcmd_send(dev_name, resp, "AT+QLWSTATUS?\r");
}


rt_err_t atcmd_qlwrecover(const char *dev_name, at_response_t resp)
{
    // AT+QLWRECOVER
    return atcmd_send(dev_name, resp, "AT+QLWRECOVER\r");
}


static void lwurc_func(struct at_client *client , const char *data, rt_size_t size)
{
    char string[size + 1];
    rt_memset(string, 0, size + 1);
    rt_memcpy(string, data, size);
    LOG_D("AT Server lwurc_func called with data: %s", string);
}


static struct at_urc nbiot_at_urc_table[] = {
    {"+QLWURC", "\r\n", lwurc_func},
};


/* just for rt msh test*/
void at_nbiot_init(void)
{
    int err = RT_EOK;

    // nb iot at client init
    err = at_client_init(AT_NBIOT_DIVICE_NAME, 512, 512);
    if (err != RT_EOK) {
        LOG_E("at_client_init err: %d\n", err);
    }

    // nb iot at client set urc table
    err = at_obj_set_urc_table(
        at_client_get(AT_NBIOT_DIVICE_NAME), 
        nbiot_at_urc_table, 
        sizeof(nbiot_at_urc_table) / sizeof(nbiot_at_urc_table[0])
    );
    if (err != RT_EOK) {
        LOG_E("at_obj_set_urc_table err: %d\n", err);
    }
}


/* just for rt msh test */
int at_Client_send(int argc, char **argv)
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

    const char *cmd = argv[1];
    if (!rt_strcmp(cmd, "atcmd_qlwconfig")) {
        // AT+QLWCONFIG=0,"leshan.eclipseprojects.io",5684,"Encortec",900,0,"863663063984075","000102030405060708090a0b0c0d0e0f"
        err = atcmd_qlwconfig(
            AT_NBIOT_DIVICE_NAME,
            resp,
            0,
            "220.180.239.212",
            8007,
            "863663063984075",
            86400,
            0,
            "863663063984075",
            "383637393937303330323935363836"
        );
    }
    else if (!rt_strcmp(cmd, "atcmd_qlwreg")) {
        err = atcmd_qlwreg(AT_NBIOT_DIVICE_NAME, resp);
    }
    else if (!rt_strcmp(cmd, "atcmd_qlwupdate")) {
        // AT+QLWUPDATE=0,1000
        err = atcmd_qlwupdate(AT_NBIOT_DIVICE_NAME, resp, 0, 1000, 0);
    }
    else if (!rt_strcmp(cmd, "atcmd_qlwaddobj")) {
        // AT+QLWADDOBJ=3304,0,4,5700,5701,5603,5604
        int resource_ids[4] = {5700, 5701, 5603, 5604};
        err = atcmd_qlwaddobj(AT_NBIOT_DIVICE_NAME, resp, 3304, 0, 4, resource_ids);
    }
    else if (!rt_strcmp(cmd, "atcmd_qlwdelobj")) {
        // AT+QLWDELOBJ=17
        err = atcmd_qlwdelobj(AT_NBIOT_DIVICE_NAME, resp, 17);
    }
    else if (!rt_strcmp(cmd, "atcmd_qlwrdrsp")) {
        // AT+QLWRDRSP=62953,1,9,0,0,1,5,"abcde",0
        err = atcmd_qlwrdrsp(AT_NBIOT_DIVICE_NAME, resp, 62953, 1, 9, 0, 0, 1, 5, "abcde", 0);
    }
    else if (!rt_strcmp(cmd, "atcmd_qlwwrrsp")) {
        // AT+QLWWRRSP=36560,2
        err = atcmd_qlwwrrsp(AT_NBIOT_DIVICE_NAME, resp, 36560, 2);
    }
    else if (!rt_strcmp(cmd, "atcmd_qlwexeresp")) {
        // AT+QLWEXERSP=39040,2
        err = atcmd_qlwexeresp(AT_NBIOT_DIVICE_NAME, resp, 39040, 2);
    }
    else if (!rt_strcmp(cmd, "atcmd_qlwobsrsp")) {
        // AT+QLWOBSRSP=624,1,9,0,0,1,5,"abcde",0
        err = atcmd_qlwobsrsp(AT_NBIOT_DIVICE_NAME, resp, 624, 1, 9, 0, 0, 1, 5, "abcde", 0);
    }
    else if (!rt_strcmp(cmd, "atcmd_qlwnotify")) {
        // AT+QLWNOTIFY=3303,1,5700,4,4,28.3,0,0,0
        err = atcmd_qlwnotify(AT_NBIOT_DIVICE_NAME, resp, 3303, 1, 5700, 4, 4, "28.3", 0, 0, 0);
    }
    else if (!rt_strcmp(cmd, "atcmd_qlwstatus")) {
        // AT+QLWSTATUS?
        err = atcmd_qlwstatus(AT_NBIOT_DIVICE_NAME, resp);
    }
    else if (!rt_strcmp(cmd, "atcmd_qlwrecover")) {
        // AT+QLWRECOVER
        err = atcmd_qlwrecover(AT_NBIOT_DIVICE_NAME, resp);
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


/* just for rt msh test*/
MSH_CMD_EXPORT(at_Client_send, AT Client send commands to AT Server and get response data);
