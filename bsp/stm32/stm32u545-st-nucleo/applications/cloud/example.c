#define ENABLE_EXAMPLE
#ifdef ENABLE_EXAMPLE


#include "nbiot.h"
#include <stdlib.h>
#include <time.h>


/* just for rt msh test */
int nbiot_at_client_send(int argc, char **argv)
{
    rt_err_t err = RT_EOK;
    rt_size_t size = 0;

    if (argc != 2) {
        LOG_E("at_cli_send [command]  - AT client send commands to AT server.\n");
        return -RT_ERROR;
    }

    at_response_t resp = RT_NULL;
    resp = at_create_resp(512, 0, rt_tick_from_millisecond(AT_WAIT_RESP_MAX_MILLISECOND));
    if (!resp) {
        LOG_E("No memory for response structure!\n");
        return -RT_ENOMEM;
    }

    const char *cmd = argv[1];
    if (!rt_strcmp(cmd, "nbiot_qiotcfg_query_productinfo")) {
        err = nbiot_qiotcfg_query_productinfo(resp);
    }
    else if (!rt_strcmp(cmd, "nbiot_qiotcfg_query_server")) {
        err = nbiot_qiotcfg_query_server(resp);
    }
    else if (!rt_strcmp(cmd, "nbiot_qiotmodeltd_set")) {
        err = nbiot_qiotmodeltd_set(resp, 0, 10, -1);
    }
    else if (!rt_strcmp(cmd, "at_client_send")) {
        at_client_t client = at_client_get(AT_NBIOT_DEVICE_NAME);
        size = at_client_obj_send(client, "{\"1\":45.6}", 10);
        LOG_D("at_client_obj_send size: %d\n", size);
    }
    else if (!rt_strcmp(cmd, "nbiot_qidnscfg_set")) {
        // AT+QIDNSCFG=0,"114.114.114.114","8.8.8.8"
        err = nbiot_atcmd_send(resp, "AT+QIDNSCFG=0,\"114.114.114.114\",\"8.8.8.8\"");
    }
    else if (!rt_strcmp(cmd, "nbiot_qiotreg_set0")) {
        err = nbiot_qiotreg_set(resp, 0);
    }
    else if (!rt_strcmp(cmd, "nbiot_qiotreg_set1")) {
        err = nbiot_qiotreg_set(resp, 1);
    }
    else if (!rt_strcmp(cmd, "nbiot_qiotreg_set2")) {
        err = nbiot_qiotreg_set(resp, 2);
    }
    else if (!rt_strcmp(cmd, "report_model_data")) {
        size = report_model_data("{\"5\":123.456}", 13);
        LOG_D("report_model_data send size: %d\n", size);
    }
    else if (!rt_strcmp(cmd, "disable_sleep_mode")) {
        // AT+QSCLK=0
        err = nbiot_atcmd_send(resp, "AT+QSCLK=0");
    }
    else if (!rt_strcmp(cmd, "enable_sleep_mode")) {
        // AT+QSCLK=0
        err = nbiot_atcmd_send(resp, "AT+QSCLK=1");
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


static rt_thread_t nbiot_start_report_test_data_thread = RT_NULL;


static void test_data_thread_handler(void *params)
{
    unsigned int seed = time(NULL);
    srand(seed);
 
    /* 生成随机数 */
    int random_number;
    char value_string[10] = {0};
    char data_string[50] = {0};

    while (1)
    {
        random_number = rand();

        rt_memset(value_string, 0, 10);
        snprintf(value_string, 10, "%d", random_number);
        value_string[3] = '.';

        LOG_D("random number is: %d; value_string is: \"%s\"", random_number, value_string);

        rt_memset(data_string, 0, 50);
        snprintf(
            data_string,
            50,
            "{\"5\":%s,\"1\":%s}",
            value_string,
            value_string
        );

        report_model_data(data_string, rt_strlen(data_string));
        rt_thread_mdelay(30000);
    }
}


void nbiot_start_report_test_data()
{
    // start urc handler thread
    nbiot_start_report_test_data_thread = rt_thread_create(
        "test_data",
        test_data_thread_handler,
        RT_NULL,
        4096,
        RT_THREAD_PRIORITY_MAX / 3 - 1,
        5
    );
    if (nbiot_start_report_test_data_thread == RT_NULL) {
        LOG_E("nbiot_start_report_test_data_thread create failed!\n");
    }
    else {
        rt_thread_startup(nbiot_start_report_test_data_thread);
    }
}


/* just for rt msh test*/
MSH_CMD_EXPORT(nbiot_start_report_test_data, AT Client report model data);
MSH_CMD_EXPORT(nbiot_at_client_send, AT Client send commands to AT Server and get response data);

#endif
