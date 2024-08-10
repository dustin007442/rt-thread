#include <rtthread.h>
#include <rtdbg.h>

#define DBG_TAG "example_http"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

static rt_device_t cat1_uart;


void cat1_uart_entry(void *params)
{

}


rt_err_t cat1_uart_init()
{
    cat1_uart = rt_device_find("uart1");
    if (cat1_uart == RT_NULL) {
        LOG_E("cat uart find failed.");
        return -RT_ERROR;
    }

    rt_thread_create("cat1_thread", cat1_uart_entry, RT_NULL, 512, 5, RT_TICK_PER_SECOND);

}

