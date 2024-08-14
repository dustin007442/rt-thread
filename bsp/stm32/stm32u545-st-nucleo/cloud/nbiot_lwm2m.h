#ifndef __NBIOT_LWM2M_H__
#define __NBIOT_LWM2M_H__

#include <rtthread.h>
#include <drv_gpio.h>
#include <rtdevice.h>
#include <at.h>
#include <finsh.h>
#include <string.h>
#include <stdio.h>


#define AT_NBIOT_DEVICE_NAME "uart1"
#define AT_STRING_MAX_SIZE 256
#define AT_WAIT_RESP_MAX_MILLISECOND 5000

#define QTH_PRODUCTINFO_PK "pe15TE"
#define QTH_PRODUCTINFO_PS "aXp5Y0hudFBkbmho"
// #define QTH_SERVER "coap://iot-south.quectelcn.com:5683"
#define QTH_SERVER "coap://iot-south.quecteleu.com:5683"
#define QTH_SERVER_TYPE 0
#define QTH_LIFETIME 86400
#define QTH_BUFFER_MODE 1
#define QTH_CONTEXTID 1
#define QTH_TSL_MODE 1
#define QTH_REG_MODE 1
#define DNS_SERVER_MASTER "114.114.114.114"
#define DNS_SERVER_BACKUP "8.8.8.8"
#define APN_NAME "lpwa.vodafone.iot"


rt_err_t nbiot_atcmd_send(at_response_t resp, const char *at_string);


rt_err_t nbiot_qiotcfg_set_productinfo(at_response_t resp, const char *pk, const char*ps);


rt_err_t nbiot_qiotcfg_query_productinfo(at_response_t resp);


rt_err_t nbiot_qiotcfg_set_server(at_response_t resp, const int server_type, const char *server_URL);


rt_err_t nbiot_qiotcfg_query_server(at_response_t resp);


rt_err_t nbiot_qiotcfg_set_lifetime(at_response_t resp, const int lifetime);


rt_err_t nbiot_qiotcfg_query_lifetime(at_response_t resp);


rt_err_t nbiot_qiotcfg_set_buffer(at_response_t resp, const int buffer_mode);


rt_err_t nbiot_qiotcfg_query_buffer(at_response_t resp);


rt_err_t nbiot_qiotcfg_set_act(at_response_t resp, const int contextID);


rt_err_t nbiot_qiotcfg_query_act(at_response_t resp);


rt_err_t nbiot_qiotcfg_set_tsl(at_response_t resp, const int tsl_mode);


rt_err_t nbiot_qiotcfg_query_tsl(at_response_t resp);


rt_err_t nbiot_qiotcfg_set_dk_ds(at_response_t resp, const char *dk, const char *ds);


rt_err_t nbiot_qiotcfg_query_dk_ds(at_response_t resp);


rt_err_t nbiot_qiotcfg_set_psk(at_response_t resp, const char *psk);


rt_err_t nbiot_qiotcfg_query_psk(at_response_t resp);


rt_err_t nbiot_qiotreg_set(at_response_t resp, const int reg_mode);


rt_err_t nbiot_qiotreg_query(at_response_t resp);


rt_err_t nbiot_qiotstate_query(at_response_t resp);


rt_err_t nbiot_qiotmcuver_set(at_response_t resp, const char *MCU_number, const char *MCU_number_version);


rt_err_t nbiot_qiotmcuver_query(at_response_t resp);


rt_err_t nbiot_qiotsend(at_response_t resp, const int mode, const int length, const char *data);


rt_err_t nbiot_qiotrd_set(at_response_t resp, const int req_length);


rt_err_t nbiot_qiotrd_query(at_response_t resp);


rt_err_t nbiot_qiotmodeltd_set(at_response_t resp, const int mode, const int length, const int PkgID);


rt_err_t nbiot_qiotmodelrd_set(at_response_t resp, const int req_length);


rt_err_t nbiot_qiotmodelrd_query(at_response_t resp);


rt_err_t nbiot_qiotinfo_set(at_response_t resp);


rt_err_t nbiot_qiotinfo_query(at_response_t resp);


rt_err_t nbiot_qiototareq_set(at_response_t resp, const int info);


rt_err_t nbiot_qiototareq_exec(at_response_t resp);


rt_err_t nbiot_qiotupdate_set(at_response_t resp, const int update_action);


rt_err_t nbiot_qiototard_set(at_response_t resp, const int start, const int length);


rt_err_t nbiot_qiotlocin_set(at_response_t resp, const int type, const char *mode);


rt_err_t nbiot_qiotlocin_query(at_response_t resp);


rt_err_t nbiot_qiotlocext_set(at_response_t resp, const char *nmea);


rt_err_t nbiot_at_client_init(void);


rt_err_t report_model_data(at_response_t resp, const char *data, rt_size_t length);


rt_err_t nbiot_at_wakeup(at_response_t resp);


rt_err_t nbiot_at_sleep(at_response_t resp);


rt_err_t nbiot_lwm2m_init();


rt_err_t nbiot_wait_network_ready();

#endif
