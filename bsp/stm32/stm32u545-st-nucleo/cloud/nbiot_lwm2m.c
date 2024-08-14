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
#include "nbiot_lwm2m.h"


#define DBG_TAG "nbiot"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>



static rt_thread_t nbiot_thread = RT_NULL;
static rt_mq_t nbiot_urc_queue = RT_NULL;
static rt_sem_t nbiot_ack_sem = RT_NULL;


rt_err_t nbiot_atcmd_send(at_response_t resp, const char *at_string)
{   
    rt_err_t result = RT_ERROR;
    at_client_t client = at_client_get(AT_NBIOT_DEVICE_NAME);
    if (client == RT_NULL) {
        LOG_E("nbiot at client not init.\n");
        return RT_ERROR;
    }
    LOG_E("nbiot_atcmd_send: %s", at_string);
    return at_obj_exec_cmd(client, resp, at_string);
}


rt_err_t nbiot_qiotcfg_set_productinfo(at_response_t resp, const char *pk, const char*ps)
{
    // AT+QIOTCFG="productinfo"[,\<pk\>,\<ps\>]
    char at_string[AT_STRING_MAX_SIZE] = {0};
    snprintf(at_string, AT_STRING_MAX_SIZE, "AT+QIOTCFG=\"productinfo\",\"%s\",\"%s\"", pk, ps);
    return nbiot_atcmd_send(resp, at_string);
}


rt_err_t nbiot_qiotcfg_query_productinfo(at_response_t resp)
{
    // AT+QIOTCFG="productinfo"
    return nbiot_atcmd_send(resp, "AT+QIOTCFG=\"productinfo\"");
}


rt_err_t nbiot_qiotcfg_set_server(at_response_t resp, const int server_type, const char *server_URL)
{
    // AT+QIOTCFG="server"[,<server_type>,<server_URL>]
    char at_string[AT_STRING_MAX_SIZE] = {0};
    snprintf(at_string, AT_STRING_MAX_SIZE, "AT+QIOTCFG=\"server\",%d,\"%s\"", server_type, server_URL);
    return nbiot_atcmd_send(resp, at_string);
}


rt_err_t nbiot_qiotcfg_query_server(at_response_t resp)
{
    // AT+QIOTCFG="server"
    return nbiot_atcmd_send(resp, "AT+QIOTCFG=\"server\"");
}


rt_err_t nbiot_qiotcfg_set_lifetime(at_response_t resp, const int lifetime)
{
    // AT+QIOTCFG="lifetime"[,<lifetime>] 
    char at_string[AT_STRING_MAX_SIZE] = {0};
    snprintf(at_string, AT_STRING_MAX_SIZE, "AT+QIOTCFG=\"lifetime\",%d", lifetime);
    return nbiot_atcmd_send(resp, at_string);
}


rt_err_t nbiot_qiotcfg_query_lifetime(at_response_t resp)
{
    // AT+QIOTCFG="lifetime"
    return nbiot_atcmd_send(resp, "AT+QIOTCFG=\"lifetime\"");
}


rt_err_t nbiot_qiotcfg_set_buffer(at_response_t resp, const int buffer_mode)
{
    // AT+QIOTCFG="buffer"[,<buffer_mode>]
    char at_string[AT_STRING_MAX_SIZE] = {0};
    snprintf(at_string, AT_STRING_MAX_SIZE, "AT+QIOTCFG=\"buffer\",%d", buffer_mode);
    return nbiot_atcmd_send(resp, at_string);
}


rt_err_t nbiot_qiotcfg_query_buffer(at_response_t resp)
{
    // AT+QIOTCFG="buffer"
    return nbiot_atcmd_send(resp, "AT+QIOTCFG=\"buffer\"");
}


rt_err_t nbiot_qiotcfg_set_act(at_response_t resp, const int contextID)
{
    // AT+QIOTCFG="act"[,<contextID>]
    char at_string[AT_STRING_MAX_SIZE] = {0};
    snprintf(at_string, AT_STRING_MAX_SIZE, "AT+QIOTCFG=\"act\",%d", contextID);
    return nbiot_atcmd_send(resp, at_string);
}


rt_err_t nbiot_qiotcfg_query_act(at_response_t resp)
{
    // AT+QIOTCFG="act"
    return nbiot_atcmd_send(resp, "AT+QIOTCFG=\"act\"");
}


rt_err_t nbiot_qiotcfg_set_tsl(at_response_t resp, const int tsl_mode)
{
    // AT+QIOTCFG="tsl"[,<tsl_mode>]
    char at_string[AT_STRING_MAX_SIZE] = {0};
    snprintf(at_string, AT_STRING_MAX_SIZE, "AT+QIOTCFG=\"tsl\",%d", tsl_mode);
    return nbiot_atcmd_send(resp, at_string);
}


rt_err_t nbiot_qiotcfg_query_tsl(at_response_t resp)
{
    // AT+QIOTCFG="tsl"[,<tsl_mode>]
    return nbiot_atcmd_send(resp, "AT+QIOTCFG=\"tsl\"");
}


rt_err_t nbiot_qiotcfg_set_dk_ds(at_response_t resp, const char *dk, const char *ds)
{
    // AT+QIOTCFG="dk_ds",[<dk>[,<ds>]]
    char at_string[AT_STRING_MAX_SIZE] = {0};
    snprintf(at_string, AT_STRING_MAX_SIZE, "AT+QIOTCFG=\"dk_ds\",\"%s\",\"%s\"", dk, ds);
    return nbiot_atcmd_send(resp, at_string);
}


rt_err_t nbiot_qiotcfg_query_dk_ds(at_response_t resp)
{
    // AT+QIOTCFG="dk_ds"
    char at_string[AT_STRING_MAX_SIZE] = {0};
    snprintf(at_string, AT_STRING_MAX_SIZE, "AT+QIOTCFG=\"dk_ds\"");
    return nbiot_atcmd_send(resp, at_string);
}


rt_err_t nbiot_qiotcfg_set_psk(at_response_t resp, const char *psk)
{
    // AT+QIOTCFG="psk",[<psk>]
    char at_string[AT_STRING_MAX_SIZE] = {0};
    snprintf(at_string, AT_STRING_MAX_SIZE, "AT+QIOTCFG=\"psk\",\"%s\"", psk);
    return nbiot_atcmd_send(resp, at_string);
}


rt_err_t nbiot_qiotcfg_query_psk(at_response_t resp)
{
    // AT+QIOTCFG="psk",[<psk>]
    char at_string[AT_STRING_MAX_SIZE] = {0};
    snprintf(at_string, AT_STRING_MAX_SIZE, "AT+QIOTCFG=\"psk\"");
    return nbiot_atcmd_send(resp, at_string);
}


rt_err_t nbiot_qiotreg_set(at_response_t resp, const int reg_mode)
{
    // AT+QIOTREG=<reg_mode>
    char at_string[AT_STRING_MAX_SIZE] = {0};
    snprintf(at_string, AT_STRING_MAX_SIZE, "AT+QIOTREG=%d", reg_mode);
    return nbiot_atcmd_send(resp, at_string);
}


rt_err_t nbiot_qiotreg_query(at_response_t resp)
{
    // AT+QIOTREG?
    return nbiot_atcmd_send(resp, "AT+QIOTREG?");
}


rt_err_t nbiot_qiotstate_query(at_response_t resp)
{
    // AT+QIOTSTATE?
    return nbiot_atcmd_send(resp, "AT+QIOTSTATE?");
}


rt_err_t nbiot_qiotmcuver_set(at_response_t resp, const char *MCU_number, const char *MCU_number_version)
{
    // AT+QIOTMCUVER=<MCU_number>[, <MCU_number_version>]
    char at_string[AT_STRING_MAX_SIZE] = {0};
    snprintf(at_string, AT_STRING_MAX_SIZE, "AT+QIOTMCUVER=\"%s\",\"%s\"", MCU_number, MCU_number_version);
    return nbiot_atcmd_send(resp, at_string);
}


rt_err_t nbiot_qiotmcuver_query(at_response_t resp)
{
    // AT+QIOTMCUVER?
    return nbiot_atcmd_send(resp, "AT+QIOTMCUVER?");
}


rt_err_t nbiot_qiotsend(at_response_t resp, const int mode, const int length, const char *data)
{
    // AT+QIOTSEND=<mode>,<length>[,<data>]
    char at_string[AT_STRING_MAX_SIZE] = {0};
    snprintf(at_string, AT_STRING_MAX_SIZE, "AT+QIOTSEND=%d,%d,\"%s\"", mode, length, data);
    return nbiot_atcmd_send(resp, at_string);
}


rt_err_t nbiot_qiotrd_set(at_response_t resp, const int req_length)
{
    // AT+QIOTRD=<req_length>
    char at_string[AT_STRING_MAX_SIZE] = {0};
    snprintf(at_string, AT_STRING_MAX_SIZE, "AAT+QIOTRD=%d", req_length);
    return nbiot_atcmd_send(resp, at_string);
}


rt_err_t nbiot_qiotrd_query(at_response_t resp)
{
    // AT+QIOTRD?
    return nbiot_atcmd_send(resp, "AT+QIOTRD?");
}


rt_err_t nbiot_qiotmodeltd_set(at_response_t resp, const int mode, const int length, const int PkgID)
{
    // AT+QIOTMODELTD=<mode>,<length>[,<PkgID>]
    char at_string[AT_STRING_MAX_SIZE] = {0};
    if (PkgID < 0) {
        snprintf(at_string, AT_STRING_MAX_SIZE, "AT+QIOTMODELTD=%d,%d", mode, length);
    }
    else {
        snprintf(at_string, AT_STRING_MAX_SIZE, "AT+QIOTMODELTD=%d,%d,%d", mode, length, PkgID);
    }
    return nbiot_atcmd_send(resp, at_string);
}


rt_err_t nbiot_qiotmodelrd_set(at_response_t resp, const int req_length)
{
    // AT+QIOTMODELRD=<req_length>
    char at_string[AT_STRING_MAX_SIZE] = {0};
    snprintf(at_string, AT_STRING_MAX_SIZE, "AT+QIOTMODELRD=%d", req_length);
    return nbiot_atcmd_send(resp, at_string);
}


rt_err_t nbiot_qiotmodelrd_query(at_response_t resp)
{
    // AT+QIOTMODELRD?
    return nbiot_atcmd_send(resp, "AT+QIOTMODELRD?");
}



rt_err_t nbiot_qiotinfo_set(at_response_t resp)
{
    // AT+QIOTINFO?
    char at_string[AT_STRING_MAX_SIZE] = {0};
    snprintf(at_string, AT_STRING_MAX_SIZE, "AT+QIOTINFO=\"up\"");
    return nbiot_atcmd_send(resp, at_string);
}


rt_err_t nbiot_qiotinfo_query(at_response_t resp)
{
    // AT+QIOTINFO?
    return nbiot_atcmd_send(resp, "AT+QIOTINFO?");
}


rt_err_t nbiot_qiototareq_set(at_response_t resp, const int info)
{
    // AT+QIOTOTAREQ[=<info>]
    char at_string[AT_STRING_MAX_SIZE] = {0};
    if (info != 0 && info != 1) {
        LOG_E("info must be 0 or 1");
        return RT_ERROR;
    }
    snprintf(at_string, AT_STRING_MAX_SIZE, "AT+QIOTOTAREQ=%d", info);
    return nbiot_atcmd_send(resp, at_string);
}


rt_err_t nbiot_qiototareq_exec(at_response_t resp)
{
    // AT+QIOTOTAREQ
    return nbiot_atcmd_send(resp, "AT+QIOTOTAREQ");
}


rt_err_t nbiot_qiotupdate_set(at_response_t resp, const int update_action)
{
    // AT+QIOTUPDATE=<update_action>
    char at_string[AT_STRING_MAX_SIZE] = {0};
    snprintf(at_string, AT_STRING_MAX_SIZE, "AT+QIOTUPDATE=%d", update_action);
    return nbiot_atcmd_send(resp, at_string);
}


rt_err_t nbiot_qiototard_set(at_response_t resp, const int start, const int length)
{
    // AT+QIOTOTARD=<start>,<length>
    char at_string[AT_STRING_MAX_SIZE] = {0};
    snprintf(at_string, AT_STRING_MAX_SIZE, "AT+QIOTOTARD=%d,%d", start, length);
    return nbiot_atcmd_send(resp, at_string);
}


rt_err_t nbiot_qiotlocin_set(at_response_t resp, const int type, const char *mode)
{
    // AT+QIOTLOCIN?
    char at_string[AT_STRING_MAX_SIZE] = {0};
    snprintf(at_string, AT_STRING_MAX_SIZE, "AT+QIOTLOCIN=%d,\"%s\"", type, mode);
    return nbiot_atcmd_send(resp, at_string);
}


rt_err_t nbiot_qiotlocin_query(at_response_t resp)
{
    // AT+QIOTLOCIN?
    return nbiot_atcmd_send(resp, "AT+QIOTLOCIN?");
}


rt_err_t nbiot_qiotlocext_set(at_response_t resp, const char *nmea)
{
    // AT+QIOTLOCEXT=<nmea1>[,<nmea2>[,...]]
    char at_string[AT_STRING_MAX_SIZE] = {0};
    snprintf(at_string, AT_STRING_MAX_SIZE, "AT+QIOTLOCEXT=\"%s\"", nmea);
    return nbiot_atcmd_send(resp, at_string);
}


static void nbiot_urc_func(struct at_client *client , const char *data, rt_size_t size)
{
    LOG_E("AT Server nbiot_urc_func called, size: %d; data: %s\n", size, data);
    rt_err_t result = rt_mq_send(nbiot_urc_queue, data, size);
    if (result != RT_EOK)
    {
        LOG_E("rt_mq_send ERR\n");
    }
}


static struct at_urc nbiot_at_urc_table[] = {
    // +QIOTEVT: <event_type>,<event_code> [,<data>]
    {"+QIOTEVT: ", "\r\n", nbiot_urc_func},
};


static void nbiot_urc_handler(void *param)
{   
    rt_err_t err;
    char data[1024] = {0};
    int event_type;
    int event_code;
    while (1)
    {
        rt_mq_recv(nbiot_urc_queue, data, 1024, RT_WAITING_FOREVER);
        LOG_E("rt_mq_recv data from queue: %s\n", data);
        // +QIOTEVT: 4,10210
        sscanf(data, "+QIOTEVT: %d,%d", &event_type, &event_code);
        LOG_E("event_type: %d; event_code: %d\n", event_type, event_code);
        if ((event_type == 4 && event_code == 10210) || (event_type == 3 && event_code == 10200)) {
            err = rt_sem_release(nbiot_ack_sem);
            LOG_E("rt_sem_release err: %d\n", err);
        }
    }
}

extern rt_err_t iot_psmeint_pin_enable(rt_uint8_t mode);
rt_err_t nbiot_at_wakeup(at_response_t resp)
{
    rt_err_t result = RT_EOK;

    // send AT before each command to make sure the UART is woken up.
    // try 3 times to wakeup NB module 
    iot_psmeint_pin_enable(PIN_LOW);
    rt_thread_mdelay(500);
    iot_psmeint_pin_enable(PIN_HIGH);
    for (int i=0; i < 3; i++) {
        result = nbiot_atcmd_send(resp, "AT");
        if (result == RT_EOK) {
            break;
        }
    }
    if (result != RT_EOK) {
        LOG_E("nbiot_at_wakeup send AT failed: %d", result);
        return result;
    }

    // make UART is always working
    return nbiot_atcmd_send(resp, "AT+QSCLK=0");
}


rt_err_t nbiot_at_sleep(at_response_t resp)
{
    // to make the module enter sleep modes
    return nbiot_atcmd_send(resp, "AT+QSCLK=1");
}


rt_err_t nbiot_qidnscfg(at_response_t resp, const char *dns_master, const char *dns_backup)
{
    rt_err_t result = RT_EOK;
    char at_string[AT_STRING_MAX_SIZE] = {0};

    resp = at_create_resp(512, 0, rt_tick_from_millisecond(AT_WAIT_RESP_MAX_MILLISECOND));
    if (!resp) {
        LOG_E("No memory for response structure!\n");
        return -RT_ENOMEM;
    }

    snprintf(at_string, AT_STRING_MAX_SIZE, "AT+QIDNSCFG=0,\"%s\",\"%s\"", dns_master, dns_backup);
    return nbiot_atcmd_send(resp, at_string);
}


int network_not_ready()
{
    rt_err_t result = RT_EOK;
    at_response_t resp;
    int n = 0;
    int stat = 0; 
    int count = 0;

    resp = at_create_resp(512, 0, rt_tick_from_millisecond(AT_WAIT_RESP_MAX_MILLISECOND));
    if (!resp) {
        LOG_E("No memory for response structure!\n");
        return -RT_ENOMEM;
    }

    for (int i=0; i < 10; i++) {
        result = nbiot_atcmd_send(resp, "AT+CEREG?");
        LOG_E("network_not_ready AT+CEREG? send result: %d", result);
        if (result == RT_EOK) {
            at_resp_parse_line_args(resp, 2, "+CEREG: %d,%d", &n, &stat);
            LOG_E("network_not_ready parse line 2, n: %d, stat: %d", n, stat);
            if (stat == 1 || stat == 5) {
                LOG_E("network_not_ready NbIot network ready.");
                at_delete_resp(resp);
                return 1;
            }
        }
    }
    at_delete_resp(resp);
    return 0;
}


rt_err_t nbiot_lwm2m_init()
{
    rt_err_t result = RT_EOK;
    at_response_t resp = RT_NULL;

    resp = at_create_resp(512, 0, rt_tick_from_millisecond(AT_WAIT_RESP_MAX_MILLISECOND));
    if (!resp) {
        LOG_E("No memory for response structure!\n");
        return -RT_ENOMEM;
    }
    nbiot_atcmd_send(resp, "ATE1");
    nbiot_atcmd_send(resp, "AT+QCFG=\"logbaudrate\",6000000");

    // nbiot_atcmd_send(RT_NULL, "AT+QIDNSCFG=0,\"114.114.114.114\",\"8.8.8.8\"");
    // set DNS
    // result = nbiot_qidnscfg(resp, DNS_SERVER_MASTER, DNS_SERVER_BACKUP);
    // if (result != RT_EOK) {
    //     LOG_E("nbiot_qidnscfg err: %d", result);
    // }

    // fot test
    if (network_not_ready()) {
        
        // result = nbiot_atcmd_send(resp, "AT+CFUN=0");
        // if (result != RT_EOK) {
        //     LOG_E("nbiot cfun0 err: %d", result);
        // }

        // set APN "lpwa.vodafone.iot"
        // result = nbiot_atcmd_send(resp, "AT+QCGDEFCONT=\"IPV4V6\",\"lpwa.vodafone.iot\"");
        // if (result != RT_EOK) {
        //     LOG_E("nbiot set apn err: %d", result);
        // }

        // set APN "quectel.tn.std"
        // result = nbiot_atcmd_send(resp, "AT+QCGDEFCONT=\"IP\",\"quectel.tn.std\"");
        // if (result != RT_EOK) {
        //     LOG_E("nbiot set apn err: %d", result);
        // }

        // result = nbiot_atcmd_send(resp, "AT+CSCON=1");
        // if (result != RT_EOK) {
        //     LOG_E("nbiot query cscon err: %d", result);
        // }

        // result = nbiot_atcmd_send(resp, "AT+QBAND=2,8,20");
        // if (result != RT_EOK) {
        //     LOG_E("nbiot set bane err: %d", result);
        // }

        // result = nbiot_atcmd_send(resp, "AT+CIPCA=3,0");
        // if (result != RT_EOK) {
        //     LOG_E("nbiot set cpi ca err: %d", result);
        // }

        // result = nbiot_atcmd_send(resp, "AT+CFUN=1");
        // if (result != RT_EOK) {
        //     LOG_E("nbiot cfun0 err: %d", result);
        // }
    }

    //  end test
    
    nbiot_wait_network_ready();

    // deregister
    result = nbiot_qiotreg_set(resp, 0);
    if (result != RT_EOK) {
        LOG_E("nbiot_qiotreg_set err: %d", result);
    }

    // set productioninfo
    result = nbiot_qiotcfg_set_productinfo(resp, QTH_PRODUCTINFO_PK, QTH_PRODUCTINFO_PS);
    if (result != RT_EOK) {
        LOG_E("nbiot_qiotcfg_set_productinfo err: %d", result);
    }

    // set server
    result = nbiot_qiotcfg_set_server(resp, QTH_SERVER_TYPE, QTH_SERVER);
    if (result != RT_EOK) {
        LOG_E("nbiot_qiotcfg_set_server err: %d", result);
    }

    // set lifetime
    result = nbiot_qiotcfg_set_lifetime(resp, QTH_LIFETIME);
    if (result != RT_EOK) {
        LOG_E("nbiot_qiotcfg_set_lifetime err: %d", result);
    }

    // set buffer
    result = nbiot_qiotcfg_set_buffer(resp, QTH_BUFFER_MODE);
    if (result != RT_EOK) {
        LOG_E("nbiot_qiotcfg_set_buffer err: %d", result);
    }

    // set act
    result = nbiot_qiotcfg_set_act(resp, QTH_CONTEXTID);
    if (result != RT_EOK) {
        LOG_E("nbiot_qiotcfg_set_act err: %d", result);
    }

    // set tsl
    result = nbiot_qiotcfg_set_tsl(resp, QTH_TSL_MODE);
    if (result != RT_EOK) {
        LOG_E("nbiot_qiotcfg_set_tsl err: %d", result);
    }

    // register
    result = nbiot_qiotreg_set(resp, 1);
    if (result != RT_EOK) {
        LOG_E("nbiot_qiotreg_set err: %d", result);
    }
    rt_err_t err = rt_sem_take(nbiot_ack_sem, rt_tick_from_millisecond(10000));
    LOG_E("register rt_sem_take err: %d\n", err);

    at_delete_resp(resp);
    return result;
}


rt_err_t nbiot_at_client_init(void)
{
    int err = RT_EOK;

    at_client_t client = at_client_get(AT_NBIOT_DEVICE_NAME);
    if (client != RT_NULL) {
        LOG_E("at_client for \"%s\" already inited!!!\n", AT_NBIOT_DEVICE_NAME);
        return -RT_ERROR;
    }

    nbiot_ack_sem = rt_sem_create("ack_sem", 0, RT_IPC_FLAG_FIFO);

    // nb iot at client init
    err = at_client_init(AT_NBIOT_DEVICE_NAME, 512, 512);
    if (err != RT_EOK) {
        LOG_E("at_client_init err: %d\n", err);
        return -RT_ERROR;
    }
    // set end sign for AT+QIOTMODELTD
    at_obj_set_end_sign(at_client_get(AT_NBIOT_DEVICE_NAME), '>');

    // set urc table
    err = at_obj_set_urc_table(
        at_client_get(AT_NBIOT_DEVICE_NAME), 
        nbiot_at_urc_table, 
        sizeof(nbiot_at_urc_table) / sizeof(nbiot_at_urc_table[0])
    );
    if (err != RT_EOK) {
        LOG_E("at_obj_set_urc_table err: %d\n", err);
        return err;
    }

    // prepare msg queue for urc
    nbiot_urc_queue = rt_mq_create("nbiot_urc_queue", 1024, 100, RT_IPC_FLAG_FIFO);
    if (nbiot_urc_queue == RT_NULL) {
        LOG_E("nbiot_urc_queue create failed!\n");
        return -RT_ERROR;
    }

    // start urc handler thread
    nbiot_thread = rt_thread_create( "nbiot_urc", nbiot_urc_handler, RT_NULL, 4096, RT_THREAD_PRIORITY_MAX / 3 - 1, 5);
    if (nbiot_thread == RT_NULL) {
        LOG_E("nbiot_thread create failed!\n");
        return -RT_ERROR;
    }
    rt_thread_startup(nbiot_thread);

    return RT_EOK;
}


rt_err_t report_model_data(at_response_t resp, const char *data, rt_size_t length)
{
    LOG_E("report_model_data: %s; length: %d\n", data, length);

    rt_err_t err = RT_EOK;
    at_client_t client = RT_NULL;

    err = nbiot_qiotmodeltd_set(resp, 100, length, -1);
    if (err != RT_EOK) {
        LOG_E("nbiot_qiotmodeltd_set failed: %d\n", err);
        return -RT_ERROR;
    }
    
    client = at_client_get(AT_NBIOT_DEVICE_NAME);
    if (client == RT_NULL) {
        LOG_E("at_client_init failed.\n");
        return -RT_ERROR;
    }

    at_client_obj_send(client, data, length);
    err = rt_sem_take(nbiot_ack_sem, rt_tick_from_millisecond(5000));
    LOG_E("rt_sem_take err: %d\n", err);
    
    return err;
}


rt_err_t nbiot_wait_network_ready()
{
    rt_err_t result = RT_EOK;
    at_response_t resp;
    int n = 0;
    int stat = 0; 
    int count = 0;

    resp = at_create_resp(512, 0, rt_tick_from_millisecond(AT_WAIT_RESP_MAX_MILLISECOND));
    if (!resp) {
        LOG_E("No memory for response structure!\n");
        return -RT_ENOMEM;
    }
    // nbiot_atcmd_send(resp, "AT+COPS=4,2,\"23415\",9");  // for test

    for (int i=0; i < 180; i++) {
        result = nbiot_atcmd_send(resp, "AT+CEREG?");
        LOG_E("AT+CEREG? send result: %d", result);
        if (result == RT_EOK) {
            at_resp_parse_line_args(resp, 2, "+CEREG: %d,%d", &n, &stat);
            LOG_E("parse line 2, n: %d, stat: %d", n, stat);
            if (stat == 1 || stat == 5) {
                LOG_E("NbIot network ready.");
                at_delete_resp(resp);
                return result;
            }
        }
        nbiot_atcmd_send(resp, "AT+QENG=0");  // for test
        nbiot_atcmd_send(resp, "AT+CSQ");  // for test
        rt_thread_mdelay(10000);
    }
    nbiot_atcmd_send(resp, "AT+QIDNSCFG=0"); // for test
    nbiot_atcmd_send(resp, "ATI"); // for test

    at_delete_resp(resp);
    LOG_E("NbIot network not ready.");
    return result;
}
