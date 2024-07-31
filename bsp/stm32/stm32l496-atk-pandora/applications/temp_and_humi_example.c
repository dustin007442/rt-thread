#include <rtthread.h>
#include <rtdevice.h>


#define DBG_TAG "TEMP_AND_HUMI"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>


#define AHT10_I2C_BUS_NAME          "i2c1"  /* 传感器连接的I2C总线设备名称 */
#define AHT10_ADDR                  0x48    /* 从机地址 */

static struct rt_i2c_bus_device *i2c_bus = RT_NULL;     /* I2C总线设备句柄 */


/* 读传感器寄存器数据 */
static rt_err_t read_regs(struct rt_i2c_bus_device *bus, rt_uint8_t len, rt_uint8_t *buf)
{
    struct rt_i2c_msg msgs;
    msgs.addr = AHT10_ADDR;
    msgs.flags = RT_I2C_RD;
    msgs.buf = buf;
    msgs.len = len;
    
    /* 调用I2C设备接口传输数据 */
    if (rt_i2c_transfer(bus, &msgs, 1) == 1)
    {
        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}


static void u403_sensor_init(const char *name)
{
    /* 查找I2C总线设备，获取I2C总线设备句柄 */
    i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(name);
    if (i2c_bus == RT_NULL)
    {
        rt_kprintf("can't find %s device!\n", name);
    }
}


void temp_and_humi_sensor_example(void)
{
    u403_sensor_init(AHT10_I2C_BUS_NAME);
    /* 等待传感器正常工作 */
    rt_thread_mdelay(2000);

}
