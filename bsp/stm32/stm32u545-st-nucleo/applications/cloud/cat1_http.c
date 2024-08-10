#include "cat1_http.h"


rt_err_t cat1_atcmd_send(at_response_t resp, const char *at_string)
{   
    rt_err_t result = RT_ERROR;
    at_client_t client = at_client_get(CAT1_AT_DEVICE_NAME);
    if (client == RT_NULL) {
        LOG_E("cat1 at client not init.\n");
        return RT_ERROR;
    }
    LOG_D("cat1_atcmd_send result: \"%s\"", at_string);
    return at_obj_exec_cmd(client, resp, at_string);
}


rt_err_t cat1_qicsgp(at_response_t resp, const int contextID, const int contextType, const char *APN,
                    const char *username, const char* password, const int authentication, const int CMDA_pwd)
{
    // AT+QICSGP=<contextID>[,<context_type>,<APN>[,<username>,<password>)[,<authentication>[,<CDMA_pwd>]]]]
    char at_string[512] = {0};
    if (contextID < 1 || contextID > 15) {
        LOG_E("cat1_qicsgp contextID not between [1, 15]");
        return RT_ERROR;
    }
    if (contextType == 1 || contextType == 2 || contextType == 3) {
        if (username != RT_NULL && password != RT_NULL) {
            if (authentication == 0 || authentication == 1 || authentication == 2 || authentication == 3) {
                if (CMDA_pwd == 0 || CMDA_pwd == 1) {
                    snprintf(at_string, 512, "AT+QICSGP=%d,%d,\"%s\",\"%s\",\"%s\",%d,%d", contextID, contextType, APN, username, password, authentication, CMDA_pwd);
                }
                else {
                    snprintf(at_string, 512, "AT+QICSGP=%d,%d,\"%s\",\"%s\",\"%s\",%d", contextID, contextType, APN, username, password, authentication);
                }
            }
            else {
                snprintf(at_string, 512, "AT+QICSGP=%d,%d,\"%s\",\"%s\",\"%s\"", contextID, contextType, APN, username, password);
            }
        }
        else {
            snprintf(at_string, 512, "AT+QICSGP=%d,%d,\"%s\"", contextID, contextType, APN);
        }
    }
    else {
        snprintf(at_string, 512, "AT+QICSGP=%d", contextID);
    }

    return cat1_atcmd_send(resp, at_string);
}


rt_err_t cat1_qiact(at_response_t resp, const int contextID)
{
    // AT+QIACT=<contextID>
    char at_string[512] = {0};
    snprintf(at_string, 512, "AT+QIACT=%d", contextID);
    return cat1_atcmd_send(resp, at_string);
}


rt_err_t cat1_qhttpcfg_set_contextid(at_response_t resp, const int contextID)
{
    // AT+QHTTPCFG="contextid"[,<contextID>]
    char at_string[512] = {0};
    snprintf(at_string, 512, "AT+QHTTPCFG=\"contextid\",%d", contextID);
    return cat1_atcmd_send(resp, at_string);
}


rt_err_t cat1_qhttpcfg_set_requestheader(at_response_t resp, const int request_header)
{
    // AT+QHTTPCFG="requestheader"[,<request_header>]
    char at_string[512] = {0};
    snprintf(at_string, 512, "AT+QHTTPCFG=\"requestheader\",%d", request_header);
    return cat1_atcmd_send(resp, at_string);
}


rt_err_t cat1_qhttpcfg_set_responseheader(at_response_t resp, const int response_header)
{
    // AT+QHTTPCFG="responseheader"[,<response_header>]
    char at_string[512] = {0};
    snprintf(at_string, 512, "AT+QHTTPCFG=\"responseheader\",%d", response_header);
    return cat1_atcmd_send(resp, at_string);
}


rt_err_t cat1_qhttpcfg_set_sslctxid(at_response_t resp, const int sslctxID)
{
    // AT+QHTTPCFG="sslctxid"[,<sslctxID>]
    char at_string[512] = {0};
    snprintf(at_string, 512, "AT+QHTTPCFG=\"sslctxid\",%d", sslctxID);
    return cat1_atcmd_send(resp, at_string);
}


rt_err_t cat1_qhttpcfg_set_contenttype(at_response_t resp, const int content_type)
{
    // AT+QHTTPCFG="contenttype"[,<content_type>]
    char at_string[512] = {0};
    snprintf(at_string, 512, "AT+QHTTPCFG=\"contenttype\",%d", content_type);
    return cat1_atcmd_send(resp, at_string);
}


rt_err_t cat1_qhttpcfg_set_autooutrsp(at_response_t resp, const int auto_outrsp)
{
    // AT+QHTTPCFG="rspout/auto"[,<auto_outrsp>]
    char at_string[512] = {0};
    snprintf(at_string, 512, "AT+QHTTPCFG=\"rspout/auto\",%d", auto_outrsp);
    return cat1_atcmd_send(resp, at_string);
}


rt_err_t cat1_qhttpcfg_set_closedind(at_response_t resp, const int closedind)
{
    // AT+QHTTPCFG="closed/ind"[,<closedind>]
    char at_string[512] = {0};
    snprintf(at_string, 512, "AT+QHTTPCFG=\"closed/ind\",%d", closedind);
    return cat1_atcmd_send(resp, at_string);
}


rt_err_t cat1_qhttpcfg_add_reqheader(at_response_t resp, const char *header_name, const char *header_str)
{
    // AT+QHTTPCFG="reqheader/add"[,<header_name>[,<header_str>]]
    char at_string[512] = {0};
    snprintf(at_string, 512, "AT+QHTTPCFG=\"reqheader/add\",\"%s\",\"%s\"", header_name, header_str);
    return cat1_atcmd_send(resp, at_string);
}


rt_err_t cat1_qhttpcfg_remove_reqheader(at_response_t resp, const char *header_name)
{
    // AT+QHTTPCFG="reqheader/remove",<header_name>
    char at_string[512] = {0};
    snprintf(at_string, 512, "AT+QHTTPCFG=\"reqheader/remove\",\"%s\"", header_name);
    return cat1_atcmd_send(resp, at_string);
}


rt_err_t cat1_qsslcfg_set_sslversion(at_response_t resp, const int SSL_ctxID, const int SSL_version)
{
    // AT+QSSLCFG="sslversion",<SSL_ctxID>[,<SSL_version>]
    char at_string[512] = {0};
    if (SSL_version < 0) {
        snprintf(at_string, 512, "AT+QSSLCFG=\"sslversion\",%d", SSL_ctxID);
    }
    else {
        snprintf(at_string, 512, "AT+QSSLCFG=\"sslversion\",%d,%d", SSL_ctxID, SSL_version);
    }
    
    return cat1_atcmd_send(resp, at_string);
}


rt_err_t cat1_qsslcfg_set_ciphersuite(at_response_t resp, const int SSL_ctxID, const int cipher_suites)
{
    // AT+QSSLCFG="ciphersuite",<SSL_ctxID>[,<cipher_suites>]
    char at_string[512] = {0};
    if (cipher_suites < 0) {
        snprintf(at_string, 512, "AT+QSSLCFG=\"ciphersuite\",%d", SSL_ctxID);
    }
    else {
        snprintf(at_string, 512, "AT+QSSLCFG=\"ciphersuite\",%d,%d", SSL_ctxID, cipher_suites);
    }

    return cat1_atcmd_send(resp, at_string);
}


rt_err_t cat1_qsslcfg_set_cacert(at_response_t resp, const int SSL_ctxID, const char *cacertpath)
{
    // AT+QSSLCFG="cacert",<SSL_ctxID>[,<cacertpath>]
    char at_string[512] = {0};
    if (cacertpath == RT_NULL) {
        snprintf(at_string, 512, "AT+QSSLCFG=\"cacert\",%d", SSL_ctxID);
    }
    else {
        snprintf(at_string, 512, "AT+QSSLCFG=\"cacert\",%d,\"%s\"", SSL_ctxID, cacertpath);
    }

    return cat1_atcmd_send(resp, at_string);
}


rt_err_t cat1_qsslcfg_set_cacertex(at_response_t resp, const int SSL_ctxID, const char *cacertpath)
{
    // AT+QSSLCFG="cacertex"[,<SSL_ctxID>[,<cacertpath>]]
    char at_string[512] = {0};
    if (cacertpath == RT_NULL) {
        snprintf(at_string, 512, "AT+QSSLCFG=\"cacertex\",%d", SSL_ctxID);
    }
    else {
        snprintf(at_string, 512, "AT+QSSLCFG=\"cacertex\",%d,\"%s\"", SSL_ctxID, cacertpath);
    }
    
    return cat1_atcmd_send(resp, at_string);
}


rt_err_t cat1_qsslcfg_set_clientcert(at_response_t resp, const int SSL_ctxID, const char *client_cert_path)
{
    // AT+QSSLCFG="clientcert",<SSL_ctxID>[,<client_cert_path>]
    char at_string[512] = {0};
    if (client_cert_path == RT_NULL) {
        snprintf(at_string, 512, "AT+QSSLCFG=\"clientcert\",%d", SSL_ctxID);
    }
    else {
        snprintf(at_string, 512, "AT+QSSLCFG=\"clientcert\",%d,\"%s\"", SSL_ctxID, client_cert_path);
    }
    return cat1_atcmd_send(resp, at_string);
}


rt_err_t cat1_qsslcfg_set_clientkey(at_response_t resp, const int SSL_ctxID, const char *client_key_path)
{
    // AT+QSSLCFG="clientkey",<SSL_ctxID>[,<client_key_path>]
    char at_string[512] = {0};
    if (client_key_path == RT_NULL) {
        snprintf(at_string, 512, "AT+QSSLCFG=\"clientkey\",%d", SSL_ctxID);
    }
    else {
        snprintf(at_string, 512, "AT+QSSLCFG=\"clientkey\",%d,\"%s\"", SSL_ctxID, client_key_path);
    }
    return cat1_atcmd_send(resp, at_string);
}


rt_err_t cat1_qsslcfg_set_seclevel(at_response_t resp, const int SSL_ctxID, const int seclevel)
{
    // AT+QSSLCFG="seclevel",<SSL_ctxID>[,<seclevel>]
    char at_string[512] = {0};
    if (seclevel < 0) {
        snprintf(at_string, 512, "AT+QSSLCFG=\"seclevel\",%d", SSL_ctxID);
    }
    else {
        snprintf(at_string, 512, "AT+QSSLCFG=\"seclevel\",%d,%d", SSL_ctxID, seclevel);
    }
    
    return cat1_atcmd_send(resp, at_string);
}


rt_err_t cat1_qsslcfg_set_ignorelocaltime(at_response_t resp, const int SSL_ctxID, const int ignore_ltime)
{
    // AT+QSSLCFG="ignorelocaltime",<SSL_ctxID>[,<ignore_ltime>]
    char at_string[512] = {0};
    if (ignore_ltime < 0) {
        snprintf(at_string, 512, "AT+QSSLCFG=\"ignorelocaltime\",%d", SSL_ctxID);
    }
    else {
        snprintf(at_string, 512, "AT+QSSLCFG=\"ignorelocaltime\",%d,%d", SSL_ctxID, ignore_ltime);
    }
    return cat1_atcmd_send(resp, at_string);
}


rt_err_t cat1_qsslcfg_set_negotiatetime(at_response_t resp, const int SSL_ctxID, const int negotiate_time)
{
    // AT+QSSLCFG="negotiatetime",<SSL_ctxID>[,<negotiate_time>]
    char at_string[512] = {0};
    if (negotiate_time < 0) {
        snprintf(at_string, 512, "AT+QSSLCFG=\"negotiatetime\",%d", SSL_ctxID);
    }
    else {
        snprintf(at_string, 512, "AT+QSSLCFG=\"negotiatetime\",%d,%d", SSL_ctxID, negotiate_time);
    }
    
    return cat1_atcmd_send(resp, at_string);
}


rt_err_t cat1_qsslcfg_set_sni(at_response_t resp, const int SSL_ctxID, const int SNI)
{
    // AT+QSSLCFG="sni",<SSL_ctxID>[,<SNI>]
    char at_string[512] = {0};
    if (SNI < 0) {
        snprintf(at_string, 512, "AT+QSSLCFG=\"sni\",%d", SSL_ctxID);
    }
    else {
        snprintf(at_string, 512, "AT+QSSLCFG=\"sni\",%d,%d", SSL_ctxID, SNI);
    }
    
    return cat1_atcmd_send(resp, at_string);
}


rt_err_t cat1_qsslcfg_set_closetimemode(at_response_t resp, const int SSL_ctxID, const int close_time_mode)
{
    // AT+QSSLCFG="closetimemode",<SSL_ctxID>[,<close_time_mode>]
    char at_string[512] = {0};
    if (close_time_mode < 0) {
        snprintf(at_string, 512, "AT+QSSLCFG=\"closetimemode\",%d", SSL_ctxID);
    }
    else {
        snprintf(at_string, 512, "AT+QSSLCFG=\"closetimemode\",%d,%d", SSL_ctxID, close_time_mode);
    }

    return cat1_atcmd_send(resp, at_string);
}


rt_err_t cat1_qsslcfg_set_ignoremulticertchainverify(at_response_t resp, const int SSL_ctxID, const int ignore_multicertchain_verify)
{
    // AT+QSSLCFG="ignoremulticertchainverify",<SSL_ctxID>[,<ignore_multicertchain_verify>]
    char at_string[512] = {0};
    if (ignore_multicertchain_verify < 0) {
        snprintf(at_string, 512, "AT+QSSLCFG=\"ignoremulticertchainverify\",%d", SSL_ctxID);
    }
    else {
        snprintf(at_string, 512, "AT+QSSLCFG=\"ignoremulticertchainverify\",%d,%d", SSL_ctxID, ignore_multicertchain_verify);
    }

    return cat1_atcmd_send(resp, at_string);
}


rt_err_t cat1_qsslcfg_set_ignoreinvalidcertsign(at_response_t resp, const int SSL_ctxID, const int ignore_invalid_certsign)
{
    // AT+QSSLCFG="ignoreinvalidcertsign",<SSL_ctxID>[,<ignore_invalid_certsign>]
    char at_string[512] = {0};
    if (ignore_invalid_certsign < 0) {
        snprintf(at_string, 512, "AT+QSSLCFG=\"ignoreinvalidcertsign\",%d", SSL_ctxID);
    }
    else {
        snprintf(at_string, 512, "AT+QSSLCFG=\"ignoreinvalidcertsign\",%d,%d", SSL_ctxID, ignore_invalid_certsign);
    }
    return cat1_atcmd_send(resp, at_string);
}


rt_err_t cat1_qsslcfg_set_session_cache(at_response_t resp, const int SSL_ctxID, const int session_cache_enable)
{
    // AT+QSSLCFG="session_cache",<SSL_ctxID>[,<session_cache_enable>]
    char at_string[512] = {0};
    if (session_cache_enable < 0) {
        snprintf(at_string, 512, "AT+QSSLCFG=\"session_cache\",%d", SSL_ctxID);
    }
    else {
        snprintf(at_string, 512, "AT+QSSLCFG=\"session_cache\",%d,%d", SSL_ctxID, session_cache_enable);
    }

    return cat1_atcmd_send(resp, at_string);
}


rt_err_t cat1_qhttpurl_set(at_response_t resp, const int URL_length, const int timeout)
{
    // AT+QHTTPURL=<URL_length>[,<timeout>]
    char at_string[512] = {0};
    snprintf(at_string, 512, "AT+QHTTPURL=%d,%d", URL_length, timeout);
    return cat1_atcmd_send(resp, at_string);
}


rt_err_t cat1_qhttpget(at_response_t resp, const int rsptime)
{
    // AT+QHTTPGET[=<rsptime>]
    char at_string[512] = {0};
    if (rsptime > 0) {
        snprintf(at_string, 512, "AT+QHTTPGET=%d", rsptime);
    }
    else {
        snprintf(at_string, 512, "AT+QHTTPGET");
    }
    
    return cat1_atcmd_send(resp, at_string);
}


rt_err_t cat1_qhttpgetex(at_response_t resp, const int rsptime, const int start_position, const int read_len)
{
    // AT+QHTTPGETEX=<rsptime>,<start_position>,<read_len>
    char at_string[512] = {0};
    snprintf(at_string, 512, "AT+QHTTPGETEX=%d,%d,%d", rsptime, start_position, read_len);
    return cat1_atcmd_send(resp, at_string);
}


rt_err_t cat1_qhttppost(at_response_t resp, const int data_length, const int input_time, const int rsptime)
{
    // AT+QHTTPPOST=<data_length>[,<input_time>,<rsptime>]
    char at_string[512] = {0};
    snprintf(at_string, 512, "AT+QHTTPPOST=%d,%d,%d", data_length, input_time, rsptime);
    return cat1_atcmd_send(resp, at_string);
}


rt_err_t cat1_qhttppostfile(at_response_t resp, const char *file_name, const int rsptime, const int post_mode)
{
    // AT+QHTTPPOSTFILE=<file_name>[,<rsptime>,<post_mode>]
    char at_string[512] = {0};
    snprintf(at_string, 512, "AT+QHTTPPOSTFILE=\"%s\",%d,%d", file_name, rsptime, post_mode);
    return cat1_atcmd_send(resp, at_string);
}


rt_err_t cat1_qhttpread(at_response_t resp, const int wait_time)
{
    // AT+QHTTPREAD[=<wait_time>]
    char at_string[512] = {0};
    snprintf(at_string, 512, "AT+QHTTPREAD=%d", wait_time);
    return cat1_atcmd_send(resp, at_string);
}


rt_err_t cat1_qhttpreadfile(at_response_t resp, const char *file_name, const int wait_time)
{
    // AT+QHTTPREADFILE=<file_name>[,<wait_time>]
    char at_string[512] = {0};
    snprintf(at_string, 512, "AT+QHTTPREADFILE=\"%s\",%d", file_name, wait_time);
    return cat1_atcmd_send(resp, at_string);
}


rt_err_t cat1_qhttpstop(at_response_t resp)
{
    // AT+QHTTPSTOP
    return cat1_atcmd_send(resp, "AT+QHTTPSTOP");
}


rt_err_t cat1_at_client_init()
{
    rt_err_t result = RT_EOK;
    at_client_t client = RT_NULL;

    client = at_client_get(CAT1_AT_DEVICE_NAME);
    if (client != RT_NULL) {
        LOG_E("cat1 at client \"\"%s\"\" already init!", CAT1_AT_DEVICE_NAME);
        goto ERROR;
    }

    result = at_client_init(CAT1_AT_DEVICE_NAME, 512, 512);
    if (result != RT_EOK) {
        LOG_E("cat1 at client init failed: %d", result);
        goto ERROR;
    }


ERROR:
    return result;
}


#define QHTTPCFG_CONTEXT_ID 1
#define QHTTPCFG_REQUEST_HEADER 0
#define QHTTPCFG_RESPONSE_HEADER 0
#define QHTTP_CONTENT_TYPE 1


rt_err_t cat1_http_init()
{
    rt_err_t result = RT_EOK;
    at_response_t resp = RT_NULL;

    resp = at_create_resp(512, 0, rt_tick_from_millisecond(5000));
    if (!resp) {
        LOG_E("No memory for response structure!\n");
        result = -RT_ENOMEM;
        goto ERROR;
    }

    result = cat1_qhttpcfg_set_contextid(resp, QHTTPCFG_CONTEXT_ID);
    if (result != RT_EOK) {
        LOG_E("cat1_qhttpcfg_set_contextid err: %d", result);
        goto ERROR;
    }

    result = cat1_qhttpcfg_set_requestheader(resp, QHTTPCFG_REQUEST_HEADER);
    if (result != RT_EOK) {
        LOG_E("cat1_qhttpcfg_set_requestheader err: %d", result);
        goto ERROR;
    }

    result = cat1_qhttpcfg_set_responseheader(resp, QHTTPCFG_RESPONSE_HEADER);
    if (result != RT_EOK) {
        LOG_E("cat1_qhttpcfg_set_responseheader err: %d", result);
        goto ERROR;
    }

    result = cat1_qhttpcfg_set_contenttype(resp, QHTTP_CONTENT_TYPE);
    if (result != RT_EOK) {
        LOG_E("cat1_qhttpcfg_set_contenttype err: %d", result);
        goto ERROR;
    }



ERROR:
    at_delete_resp(resp);
    return result;
}
