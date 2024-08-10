#include "nbiot_lwm2m.h"
#include <stdlib.h>
#include <time.h>
#include "IIC_sensors.h"
#include "board_pin.h"
#include "gnss.h"
#include "lpm.h"
#include "adxl372.h"



MSH_CMD_EXPORT(nbiot_at_client_init, nbiot_at_client_init);


/* just for rt msh test */
int nbiot_at_client_send(int argc, char **argv)
{
    rt_err_t result = RT_EOK;
    rt_size_t size = 0;

    if (argc < 2) {
        LOG_E("at_cli_send [command] [args] - AT client send commands to AT server.\n");
        return -RT_ERROR;
    }

    at_response_t resp = RT_NULL;
    resp = at_create_resp(512, 0, rt_tick_from_millisecond(AT_WAIT_RESP_MAX_MILLISECOND));
    if (!resp) {
        LOG_E("No memory for response structure!\n");
        return -RT_ENOMEM;
    }

    const char *at_string = argv[1];
    LOG_D("get at_string: %s", at_string);

    result = nbiot_atcmd_send(resp, at_string);

    LOG_D("atcmd_send result: %d\n", result);
    char *line_ptr = resp->buf;
    for (int i=1; i <= resp->line_counts; i++) {
        LOG_D("recv line %d: %s", i, line_ptr);
        line_ptr += rt_strlen(line_ptr) + 1;
    }

    at_delete_resp(resp);

    return result;
}

MSH_CMD_EXPORT(nbiot_at_client_send, AT Client send commands to AT Server and get response data);


rt_err_t nbiot_report_data_oneshot(
    float temp1, 
    float temp2,
    float temp3,
    float humi, 
    float lat, 
    float lng,
    float acc_x,
    float acc_y,
    float acc_z
)
{
    at_response_t resp = RT_NULL;
    resp = at_create_resp(512, 0, rt_tick_from_millisecond(AT_WAIT_RESP_MAX_MILLISECOND));
    if (!resp) {
        LOG_E("No memory for response structure!\n");
        return -RT_ENOMEM;
    }

    // init at client first
    nbiot_at_client_init();

    // make sure the UART is woken up
    nbiot_at_wakeup(resp);

    // init nb lwm2m
    nbiot_lwm2m_init();
    
    // wait network ready
    nbiot_wait_network_ready();
    
    // report model data
    rt_err_t err;
    char data_string[512] = {0};
    LOG_D("temp1: %f; temp2: %f; temp3: %f; humi: %f; lat: %f; lng: %f; acc_x: %f; acc_y: %f; acc_z: %f", temp1, temp2, temp3, humi, lat, lng, acc_x, acc_y, acc_z);
    rt_memset(data_string, 0, 100);
    snprintf(
        data_string, 
        512, 
        "{\"1\":%f,\"5\":%f,\"11\":%f,\"2\":%f,\"6\":%f,\"7\":%f,\"8\":%f,\"9\":%f,\"10\":%f}", 
        temp1, temp2, temp3, humi, lat, lng, acc_x, acc_y, acc_z
    );
    err = report_model_data(resp, data_string, rt_strlen(data_string));
    LOG_D("report_model_data err: %d\n", err);
    
    // enable sleep mode again
    nbiot_at_sleep(resp);

    return RT_EOK;
}


void test_nbiot_report_data(int argc, char **argv)
{
    float temp1 = 0;
    float temp2 = 0;
    float temp3 = 0;
    float humi = 0;
    float lat = 0;
    float lng = 0;
    float acc_x = 0;
    float acc_y = 0;
    float acc_z = 0;

    scanf(argv[1], "%f", &temp1);
    scanf(argv[2], "%f", &temp2);
    scanf(argv[3], "%f", &humi);
    scanf(argv[4], "%f", &lat);
    scanf(argv[5], "%f", &lng);
    scanf(argv[6], "%f", &acc_x);
    scanf(argv[7], "%f", &acc_y);
    scanf(argv[8], "%f", &acc_z);

    nbiot_report_data_oneshot(temp1, temp2, temp3, humi, lat, lng, acc_x, acc_y, acc_y);
}

MSH_CMD_EXPORT(test_nbiot_report_data, AT Client report model data just One shot);


iic_sensor_t i2c_device;

void start_demo_test()
{   
    rt_err_t result = RT_EOK;
    rt_err_t res;
    int cnt = 0;
    struct adxl372_xyz xyz = {0};

    board_pins_init();

    antenna_active_pin_enable(PIN_HIGH);  // power on antenna
    sensor_pwron_pin_enable(PIN_HIGH);
    sim_select_pin_enable(PIN_LOW);  // low for sim1
    nb_cat1_rf_pin_enable(PIN_LOW);  // low for nbiot rf
    intn_ext_ant_pin_enable(PIN_LOW);
    flash_pwron_pin_enable(PIN_HIGH);


    // cat1_power_on(void);
    cat1_power_off();
    nbiot_power_on();
    esp32_power_on();
    esp32_en_on();
    rt_hw_spi_adxl372_init();
    adxl372_init();


    gnss_open();
    rt_thread_mdelay(300);

    i2c_device = iic_sensors_init("i2c1");
    rt_thread_mdelay(100);

    float temp1 = 0;
    float temp2 = 0;
    float temp3 = 0;
    float humi = 0;
    float lat = 0;
    float lng = 0;
    float acc_x = 0;
    float acc_y = 0;
    float acc_z = 0;
    lwgps_t gnss_data;

    // while (cnt < 60)
    // {
    //     result = gnss_read_data(&gnss_data);
    //     if (result == RT_EOK) {
    //         LOG_D("GNSS Date: %d-%d-%d %d:%d:%d\r\n", gnss_data.year, gnss_data.month, gnss_data.date, 
    //                 gnss_data.hours, gnss_data.minutes, gnss_data.seconds);
    //         LOG_D("Valid status: %d\r\n", gnss_data.is_valid);
    //         LOG_D("Latitude: %f degrees\r\n", gnss_data.latitude);
    //         LOG_D("Longitude: %f degrees\r\n", gnss_data.longitude);
    //         LOG_D("Altitude: %f meters\r\n", gnss_data.altitude);
    //         if (gnss_data.is_valid) {
    //             lat = gnss_data.latitude;
    //             lng = gnss_data.longitude;
    //             LOG_D("lat: %f; lng: %f", lat, lng);
    //             break;
    //         }
    //     }
    //     cnt++;
    //     rt_thread_mdelay(1000); //at least 300 ms
    // }


    // cnt = 0;
    // while (cnt < 30)
    // {
    //     res = adxl372_query_xyz(&xyz);
    //     if (res == RT_EOK)
    //     {
    //         LOG_D("zyx.x %f, zyx.y %f, zyx.z %f", xyz.x, xyz.y, xyz.z);
    //         acc_x = xyz.x;
    //         acc_y = xyz.y;
    //         acc_z = xyz.z;
    //         break;
    //     }
    //     cnt++;
    //     rt_thread_mdelay(1000);
    // }

    temp1 = tmp116_1_read_temperature(i2c_device);
    temp2 = tmp116_2_read_temperature(i2c_device);
    temp3 = hdc3021_read_temperature(i2c_device);
    humi = hdc3021_read_humidity(i2c_device);

    nbiot_report_data_oneshot(temp1, temp2, temp3, humi, lat, lng, acc_x, acc_y, acc_y);

    gnss_close();
    iic_sensors_deinit(i2c_device);

    nbiot_power_off();
    esp32_power_off();
    esp32_en_off();

    antenna_active_pin_enable(PIN_LOW);
    sensor_pwron_pin_enable(PIN_LOW);
    sim_select_pin_enable(PIN_LOW);
    nb_cat1_rf_pin_enable(PIN_LOW);
    intn_ext_ant_pin_enable(PIN_LOW);
    flash_pwron_pin_enable(PIN_LOW);


    set_rtc_wakeup(10*60);
    shut_down();
}


MSH_CMD_EXPORT(start_demo_test, start_demo_test);
