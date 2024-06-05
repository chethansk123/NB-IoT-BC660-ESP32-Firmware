#ifndef __BC660_H__
#define __BC660_H__
#include<stdint.h>



typedef enum 
{
    DISABLE_NETWORK_REGISTRATION,
    ENABLE_NETWORK_REGISTRATION,
    ENABLE_NETWORK_LOCATION_INFO,
    ENABLE_NETWORK_LOCATION_EMM_INFO,
    PSM_ENABLE_NETWORK_LOCATION_INFO,
    PSM_NETWORK_LOCATION_EMM_INFO,
}L1_bc660_EPS_network_registration_t;


typedef enum
{
    DISABLE_SLEEP_MODE,
    ENABLE_LIGHT_SLEEP_DEEP_SLEEP,
    ENABLE_LIGHT_SLEEP,

}L1_bc660_sleep_mode_t;

typedef struct
{
    uint8_t AT_respose_buff[200]; 
    uint8_t *AT_send_buff;
    

}AT_commad_t;

void L1_bc660_configure_sleep_mode(L1_bc660_sleep_mode_t mode);
void L1_bc660_network_registration_URC(L1_bc660_EPS_network_registration_t status);
void L1_bc660_ssl_configuration(uint8_t contextID, uint8_t  connectID, uint8_t  sslversion);
void L1_bc660_MQTT_configuration(uint8_t MQTT_socket, uint8_t  SSL_status, uint8_t  ssl_contextid, uint8_t ssl_connectid,uint8_t version);
void L1_bc660_MQTT_clienconnection(uint8_t MQTT_socket,uint8_t host_name[],uint32_t port);
void L1_bc660_MQTT_Publish(uint8_t MQTT_socket, uint8_t msgID, uint32_t QoS, uint8_t retain, char *topic, uint32_t msg_lenght, char *msg);

#endif