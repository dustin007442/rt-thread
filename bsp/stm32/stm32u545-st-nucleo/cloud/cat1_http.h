#ifndef __CAT1_HTTP_H__
#define __CAT1_HTTP_H__

#include <rtthread.h>
#include <at.h>
#include <stdio.h>

#define DBG_TAG "http"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>


#define CAT1_AT_DEVICE_NAME "uart1"


rt_err_t cat1_atcmd_send(at_response_t resp, const char *at_string);

rt_err_t cat1_qicsgp(at_response_t resp, const int contextID, const int contextType, const char *APN,
                    const char *username, const char* password, const int authentication, const int CMDA_pwd);

rt_err_t cat1_qiact(at_response_t resp, const int contextID);

rt_err_t cat1_qhttpcfg_set_contextid(at_response_t resp, const int contextID);

rt_err_t cat1_qhttpcfg_set_requestheader(at_response_t resp, const int request_header);

rt_err_t cat1_qhttpcfg_set_responseheader(at_response_t resp, const int response_header);

rt_err_t cat1_qhttpcfg_set_sslctxid(at_response_t resp, const int sslctxID);

rt_err_t cat1_qhttpcfg_set_contenttype(at_response_t resp, const int content_type);

rt_err_t cat1_qhttpcfg_set_autooutrsp(at_response_t resp, const int auto_outrsp);

rt_err_t cat1_qhttpcfg_set_closedind(at_response_t resp, const int closedind);

rt_err_t cat1_qhttpcfg_add_reqheader(at_response_t resp, const char *header_name, const char *header_str);

rt_err_t cat1_qhttpcfg_remove_reqheader(at_response_t resp, const char *header_name);

rt_err_t cat1_qsslcfg_set_sslversion(at_response_t resp, const int SSL_ctxID, const int SSL_version);

rt_err_t cat1_qsslcfg_set_ciphersuite(at_response_t resp, const int SSL_ctxID, const int cipher_suites);

rt_err_t cat1_qsslcfg_set_cacert(at_response_t resp, const int SSL_ctxID, const char *cacertpath);

rt_err_t cat1_qsslcfg_set_cacertex(at_response_t resp, const int SSL_ctxID, const char *cacertpath);

rt_err_t cat1_qsslcfg_set_clientcert(at_response_t resp, const int SSL_ctxID, const char *client_cert_path);

rt_err_t cat1_qsslcfg_set_clientkey(at_response_t resp, const int SSL_ctxID, const char *client_key_path);

rt_err_t cat1_qsslcfg_set_seclevel(at_response_t resp, const int SSL_ctxID, const int seclevel);

rt_err_t cat1_qsslcfg_set_ignorelocaltime(at_response_t resp, const int SSL_ctxID, const int ignore_ltime);

rt_err_t cat1_qsslcfg_set_negotiatetime(at_response_t resp, const int SSL_ctxID, const int negotiate_time);

rt_err_t cat1_qsslcfg_set_sni(at_response_t resp, const int SSL_ctxID, const int SNI);

rt_err_t cat1_qsslcfg_set_closetimemode(at_response_t resp, const int SSL_ctxID, const int close_time_mode);

rt_err_t cat1_qsslcfg_set_ignoremulticertchainverify(at_response_t resp, const int SSL_ctxID, const int ignore_multicertchain_verify);

rt_err_t cat1_qsslcfg_set_ignoreinvalidcertsign(at_response_t resp, const int SSL_ctxID, const int ignore_invalid_certsign);

rt_err_t cat1_qsslcfg_set_session_cache(at_response_t resp, const int SSL_ctxID, const int session_cache_enable);

rt_err_t cat1_qhttpurl_set(at_response_t resp, const int URL_length, const int timeout);

rt_err_t cat1_qhttpget(at_response_t resp, const int rsptime);

rt_err_t cat1_qhttpgetex(at_response_t resp, const int rsptime, const int start_position, const int read_len);

rt_err_t cat1_qhttppost(at_response_t resp, const int data_length, const int input_time, const int rsptime);

rt_err_t cat1_qhttppostfile(at_response_t resp, const char *file_name, const int rsptime, const int post_mode);

rt_err_t cat1_qhttpread(at_response_t resp, const int wait_time);

rt_err_t cat1_qhttpreadfile(at_response_t resp, const char *file_name, const int wait_time);

rt_err_t cat1_qhttpstop(at_response_t resp);

rt_err_t cat1_at_client_init();

rt_err_t cat1_http_init();


#endif
