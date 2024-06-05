
#include"main.h"
#include"mcal_i2c.h"
#include "esp_log.h"
#include "mcal_uart.h"
#include<stdio.h>
#include<string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "L1_bc660.h"
//#include "L1_stc3117.h"

#define BC660

#define ERROR   0
#define SUCCESS 1

int size=0;
uint8_t selection[100]={0};
static const char *TAG = "STC3117_EVENT";

uint8_t  host_name[] = "\"aht1gz8r8x4vg-ats.iot.us-east-1.amazonaws.com\"";

void app_main() {
product sense2_0;
uint8_t status;
//float battery_voltage= 0;

i2c_master_init();
HTS221_init();
set_HTS221_power_mode(HTS221_POWER_MODE_ACTIVE);
mcal_uartinit();

#ifdef BC660
L1_bc660_configure_sleep_mode(DISABLE_SLEEP_MODE);
//L1_bc660_ssl_configuration(0,0,2);
//L1_bc660_MQTT_configuration(3,1,0,0,1);
//L1_bc660_MQTT_clienconnection(3,host_name,8883);
#endif

//chip_initialization_test();
//L1_voltage_GasGuage_intialization();
while(1)
{

  
    #if 0
    //L1_read_voltage(&battery_voltage);
    ESP_LOGI(TAG,"battery temperature=%.2fV", battery_voltage);
    vTaskDelay(5000/portTICK_RATE_MS);
    #endif


   
    #ifdef BC660
    L1_bc660_configure_sleep_mode(DISABLE_SLEEP_MODE);
    L1_bc660_MQTT_Publish(3,0,0,0,"\"check\"",9,"{\"a\":\"1\"}");
    L1_bc660_configure_sleep_mode(ENABLE_LIGHT_SLEEP_DEEP_SLEEP);
    vTaskDelay(1000);

    #endif

    #if 0
    read_HTS221_temperature(&sense2_0.sensor_data.temperature_data);
    read_HTS221_humidity(&sense2_0.sensor_data.humidity_data);
    for (int i=0;i<0xFFFFFF;i++);
     //ESP_LOGI(TAG,"temperature=%d", sense2_0.sensor_data.temperature_data);
    // ESP_LOGI(TAG,"humidity=%d%%", sense2_0.sensor_data.humidity_data);
    #endif

}

}