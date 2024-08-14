#include <rtthread.h>
#include "cat1_http.h"
#include "board_pin.h"
#include "lpm.h"


MSH_CMD_EXPORT(cat1_at_client_init, cat1_at_client_init);


int cat1_at_client_send(int argc, char **argv)
{
    rt_err_t result = RT_EOK;
    rt_size_t size = 0;

    if (argc < 2) {
        LOG_E("at_cli_send [command] [args] - AT client send commands to AT server.\n");
        return -RT_ERROR;
    }

    at_response_t resp = RT_NULL;
    resp = at_create_resp(512, 0, rt_tick_from_millisecond(5000));
    if (!resp) {
        LOG_E("No memory for response structure!\n");
        return -RT_ENOMEM;
    }

    const char *at_string = argv[1];
    LOG_D("get at_string: %s", at_string);

    result = cat1_atcmd_send(resp, at_string);

    LOG_D("atcmd_send result: %d\n", result);
    char *line_ptr = resp->buf;
    for (int i=1; i <= resp->line_counts; i++) {
        LOG_D("recv line %d: %s", i, line_ptr);
        line_ptr += rt_strlen(line_ptr) + 1;
    }

    at_delete_resp(resp);

    return result;
}

MSH_CMD_EXPORT(cat1_at_client_send, cat1_at_client_send);


void init_pins()
{
    cat1_pwron_pin_init();
    nbiot_pwron_pin_init();
}
MSH_CMD_EXPORT(init_pins, init_pins);


void pwron_cat1()
{
    rt_err_t result;
    result = nbiot_power_off();
    LOG_D("nbiot_power_off result: %d", result);
    result = cat1_power_on();
    LOG_D("cat1_power_on result: %d", result);
}
MSH_CMD_EXPORT(pwron_cat1, pwron_cat1);


void pwroff_cat1()
{
    rt_err_t result;
    result = cat1_power_off();
    LOG_D("cat1_power_off result: %d", result);
    result = nbiot_power_on();
    LOG_D("nbiot_power_on result: %d", result);
}
MSH_CMD_EXPORT(pwroff_cat1, pwroff_cat1);

MSH_CMD_EXPORT(cat1_http_init, cat1_http_init);
