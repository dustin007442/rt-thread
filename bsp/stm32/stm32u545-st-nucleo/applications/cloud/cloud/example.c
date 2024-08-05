#define ENABLE_EXAMPLE
#ifdef ENABLE_EXAMPLE


#include "nbiot.h"
#include <stdlib.h>
#include <time.h>
#include "IIC_sensors.h"


static iic_sensor_t i2c_dev = RT_NULL;


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
    else if (!rt_strcmp(cmd, "at")) {
        // AT
        err = nbiot_atcmd_send(resp, "AT");
    }
    else if (!rt_strcmp(cmd, "nbiot_at_client_init")) {
        // init at client
        nbiot_at_client_init();
    }
    else if (!rt_strcmp(cmd, "nbiot_at_wakeup")) {
        // wakeup
        err = nbiot_at_wakeup();
    }
    else if (!rt_strcmp(cmd, "nbiot_at_sleep")) {
        // sleep
        err = nbiot_at_sleep();
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
    
    at_delete_resp(resp);

    return err;
}

MSH_CMD_EXPORT(nbiot_at_client_send, AT Client send commands to AT Server and get response data);


static void test_data_thread_handler(void *params)
{
    rt_err_t err;
    char data_string[100] = {0};
    while (1)
    {
        float data1 = tmp116_1_read_humidity(i2c_dev);
        float data2 = tmp116_2_read_humidity(i2c_dev);

        LOG_D("data1: %f; data2: %f", data1, data2);

        rt_memset(data_string, 0, 50);
        snprintf( data_string, 50, "{\"5\":%0.2f,\"1\":%0.2f}", data1, data2);

        nbiot_at_wakeup();
        err = report_model_data(data_string, rt_strlen(data_string));
        LOG_D("report_model_data err: %d\n", err);
        nbiot_at_sleep();
        rt_thread_mdelay(30000);
    }
}


static rt_thread_t nbiot_start_report_test_data_thread = RT_NULL;

void nbiot_start_report_test_data()
{
    i2c_dev = iic_sensors_init("i2c1");
    nbiot_at_client_init();

    rt_thread_mdelay(3000);  // for sensors data ready

    // start urc handler thread
    nbiot_start_report_test_data_thread = rt_thread_create( "test_data", test_data_thread_handler, RT_NULL, 4096, RT_THREAD_PRIORITY_MAX / 3 - 1, 5);
    if (nbiot_start_report_test_data_thread == RT_NULL) {
        LOG_E("nbiot_start_report_test_data_thread create failed!\n");
    }
    else {
        rt_thread_startup(nbiot_start_report_test_data_thread);
    }
}

MSH_CMD_EXPORT(nbiot_start_report_test_data, AT Client report model data);


//  for test in sleep mode
void nbiot_report_data_oneshot()
{
    i2c_dev = iic_sensors_init("i2c1");
    nbiot_at_client_init();

    // make sure the UART is woken up
    nbiot_at_wakeup();
    // set DNS
    nbiot_atcmd_send(RT_NULL, "AT+QIDNSCFG=0,\"114.114.114.114\",\"8.8.8.8\"");

    // sleep for sensors data ready, just for test
    rt_thread_mdelay(3000);
    
    // report model data
    rt_err_t err;
    char data_string[100] = {0};
    float data1 = tmp116_1_read_humidity(i2c_dev);
    float data2 = tmp116_2_read_humidity(i2c_dev);
    LOG_D("data1: %f; data2: %f", data1, data2);
    rt_memset(data_string, 0, 100);
    snprintf(data_string, 100, "{\"5\":%0.2f,\"1\":%0.2f}", data1, data2);
    err = report_model_data(data_string, rt_strlen(data_string));
    LOG_D("report_model_data err: %d\n", err);
    
    // enable sleep mode again
    nbiot_at_sleep();
}

MSH_CMD_EXPORT(nbiot_report_data_oneshot, AT Client report model data just One shot);


#endif
