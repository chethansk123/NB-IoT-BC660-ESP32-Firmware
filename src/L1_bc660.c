#include "L1_bc660.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mcal_uart.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_log.h"

#define NULL ((void *)0)

#define DEBUG_PORT

static const char *TAG1 = "AT_COMMAND";

extern AT_commad_t AT_command;
uint8_t flag;

uint8_t MQTT_clientconnectionflag=0;

//char *host_name= malloc(100*sizeof(char));//= "\"a46qh220m6nt9-ats.iot.eu-central-1.amazonaws.com\"";
//*host_name = "\"a46qh220m6nt9-ats.iot.eu-central-1.amazonaws.com\"";

void L1_bc660_configure_sleep_mode(L1_bc660_sleep_mode_t mode)
{
    
    char *print_string = NULL;
    char *ok_string = "OK";
    uint16_t size = 0;
    if (mode == 0)
    {
        while (print_string == NULL)
        {
            uart_write_bytes(UART1, (const char *)"AT\r", 3);
            vTaskDelay(100);
            if(flag == 1)
            {
            size = uart_read_bytes(UART1, AT_command.AT_respose_buff, 100, 200 / portTICK_RATE_MS);
            print_string = strstr((const char *)AT_command.AT_respose_buff, ok_string);
            ESP_LOGI(TAG1, "%s",AT_command.AT_respose_buff);
            }
            
        }
        if (print_string != NULL)
        {
          //  #ifdef DEBUG_PORT
           // uart_write_bytes(UART0, (const char *)AT_command.AT_respose_buff, size);
           // #endif
            print_string = NULL;
            memset(AT_command.AT_respose_buff, 0, 200);
        }

        while (print_string == NULL)
        {
            uart_write_bytes(UART1, (const char *)"AT+QSCLK=0\r", 11);
            vTaskDelay(100);
          if(flag == 1)
            {
            size = uart_read_bytes(UART1, AT_command.AT_respose_buff, 100, 200 / portTICK_RATE_MS);
            print_string = strstr((const char *)AT_command.AT_respose_buff, ok_string);
            ESP_LOGI(TAG1, "%s",AT_command.AT_respose_buff);
            }
        }
        if(print_string != NULL)
        {
           /// #ifdef DEBUG_PORT
           // uart_write_bytes(UART0, (const char *)AT_command.AT_respose_buff, size);
           // #endif
            print_string = NULL;
            memset(AT_command.AT_respose_buff, 0, 200);
        }
        
    }
    else if (mode == 1)
    {
        while (print_string == NULL)
        {
            uart_write_bytes(UART1, (const char *)"AT+QSCLK=1\r", 11);
            vTaskDelay(100);
           if(flag == 1)
            {
            size = uart_read_bytes(UART1, AT_command.AT_respose_buff, 100, 200 / portTICK_RATE_MS);
            print_string = strstr((const char *)AT_command.AT_respose_buff, ok_string);
            ESP_LOGI(TAG1, "%s",AT_command.AT_respose_buff);
            }
        }
        if(print_string != NULL)
        {
           // #ifdef DEBUG_PORT
           // uart_write_bytes(UART0, (const char *)AT_command.AT_respose_buff, size);
           // #endif
            print_string = NULL;
            memset(AT_command.AT_respose_buff, 0, 200);
        }

    }
    else if (mode == 2)
    {
        while (print_string == NULL)
        {
            uart_write_bytes(UART1, (const char *)"AT+QSCLK=2\r", 11);
            vTaskDelay(100);
          if(flag == 1)
            {
            size = uart_read_bytes(UART1, AT_command.AT_respose_buff, 100, 200 / portTICK_RATE_MS);
            print_string = strstr((const char *)AT_command.AT_respose_buff, ok_string);
            ESP_LOGI(TAG1, "%s",AT_command.AT_respose_buff);
            }
        }
        if(print_string != NULL)
        {
          //  #ifdef DEBUG_PORT
           // uart_write_bytes(UART0, (const char *)AT_command.AT_respose_buff, size);
           // #endif
            print_string = NULL;
            memset(AT_command.AT_respose_buff, 0, 200);
        }
    }
}


void L1_bc660_network_registration_URC(L1_bc660_EPS_network_registration_t status)
{
    char *print_string = NULL;
    char *ok_string = "OK";
    uint16_t size = 0;
    
    if(status == 0)
    {
        while (print_string == NULL)
        {
            uart_write_bytes(UART1, (const char *)"AT\r", 3);
            size = uart_read_bytes(UART1, AT_command.AT_respose_buff, 100, 200 / portTICK_RATE_MS);
            print_string = strstr((const char *)AT_command.AT_respose_buff, ok_string);
            ESP_LOGI(TAG1, "AT error");
        }
        if (print_string != NULL)
        {
            #ifdef DEBUG_PORT
            uart_write_bytes(UART0, (const char *)AT_command.AT_respose_buff, size);
            #endif
            print_string = NULL;
            memset(AT_command.AT_respose_buff, 0, 200);
        }
         while (print_string == NULL)
        {
            uart_write_bytes(UART1, (const char *)"AT+CEREG=0\r", 11);
            size = uart_read_bytes(UART1, AT_command.AT_respose_buff, 100, 200 / portTICK_RATE_MS);
            print_string = strstr((const char *)AT_command.AT_respose_buff, ok_string);
            ESP_LOGI(TAG1, "AT+CEREG=0 ERROR");
        }
        if(print_string != NULL)
        {
            #ifdef DEBUG_PORT
            uart_write_bytes(UART0, (const char *)AT_command.AT_respose_buff, size);
            #endif
            print_string = NULL;
            memset(AT_command.AT_respose_buff, 0, 200);
        }
    }
    else if(status == 1)
    {
        while (print_string == NULL)
        {
            uart_write_bytes(UART1, (const char *)"AT\r", 3);
            size = uart_read_bytes(UART1, AT_command.AT_respose_buff, 100, 200 / portTICK_RATE_MS);
            print_string = strstr((const char *)AT_command.AT_respose_buff, ok_string);
            ESP_LOGI(TAG1, "AT error");
        }
        if (print_string != NULL)
        {
            #ifdef DEBUG_PORT
            uart_write_bytes(UART0, (const char *)AT_command.AT_respose_buff, size);
            #endif
            print_string = NULL;
            memset(AT_command.AT_respose_buff, 0, 200);
        }
         while (print_string == NULL)
        {
            uart_write_bytes(UART1, (const char *)"AT+CEREG=1\r", 11);
            size = uart_read_bytes(UART1, AT_command.AT_respose_buff, 100, 200 / portTICK_RATE_MS);
            print_string = strstr((const char *)AT_command.AT_respose_buff, ok_string);
            ESP_LOGI(TAG1, "AT+CEREG=1 ERROR");
        }
        if(print_string != NULL)
        {
            #ifdef DEBUG_PORT
            uart_write_bytes(UART0, (const char *)AT_command.AT_respose_buff, size);
            #endif
            print_string = NULL;
            memset(AT_command.AT_respose_buff, 0, 200);
        }

    }
    else if(status == 2)
    {
        while (print_string == NULL)
        {
            uart_write_bytes(UART1, (const char *)"AT\r", 3);
            size = uart_read_bytes(UART1, AT_command.AT_respose_buff, 100, 200 / portTICK_RATE_MS);
            print_string = strstr((const char *)AT_command.AT_respose_buff, ok_string);
            ESP_LOGI(TAG1, "AT error");
        }
        if (print_string != NULL)
        {
            #ifdef DEBUG_PORT
            uart_write_bytes(UART0, (const char *)AT_command.AT_respose_buff, size);
            #endif
            print_string = NULL;
            memset(AT_command.AT_respose_buff, 0, 200);
        }
         while (print_string == NULL)
        {
            uart_write_bytes(UART1, (const char *)"AT+CEREG=2\r", 11);
            size = uart_read_bytes(UART1, AT_command.AT_respose_buff, 100, 200 / portTICK_RATE_MS);
            print_string = strstr((const char *)AT_command.AT_respose_buff, ok_string);
            ESP_LOGI(TAG1, "AT+CEREG=2 ERROR");
        }
        if(print_string != NULL)
        {
            #ifdef DEBUG_PORT
            uart_write_bytes(UART0, (const char *)AT_command.AT_respose_buff, size);
            #endif
            print_string = NULL;
            memset(AT_command.AT_respose_buff, 0, 200);
        }

    }
    else if(status == 3)
    {
        while (print_string == NULL)
        {
            uart_write_bytes(UART1, (const char *)"AT\r", 3);
            size = uart_read_bytes(UART1, AT_command.AT_respose_buff, 100, 200 / portTICK_RATE_MS);
            print_string = strstr((const char *)AT_command.AT_respose_buff, ok_string);
            ESP_LOGI(TAG1, "AT error");
        }
        if (print_string != NULL)
        {
            #ifdef DEBUG_PORT
            uart_write_bytes(UART0, (const char *)AT_command.AT_respose_buff, size);
            #endif
            print_string = NULL;
            memset(AT_command.AT_respose_buff, 0, 200);
        }
         while (print_string == NULL)
        {
            uart_write_bytes(UART1, (const char *)"AT+CEREG=3\r", 11);
            size = uart_read_bytes(UART1, AT_command.AT_respose_buff, 100, 200 / portTICK_RATE_MS);
            print_string = strstr((const char *)AT_command.AT_respose_buff, ok_string);
            ESP_LOGI(TAG1, "AT+CEREG=3 ERROR");
        }
        if(print_string != NULL)
        {
            #ifdef DEBUG_PORT
            uart_write_bytes(UART0, (const char *)AT_command.AT_respose_buff, size);
            #endif
            print_string = NULL;
            memset(AT_command.AT_respose_buff, 0, 200);
        }

    }
    else if(status == 4)
    {
        while (print_string == NULL)
        {
            uart_write_bytes(UART1, (const char *)"AT\r", 3);
            size = uart_read_bytes(UART1, AT_command.AT_respose_buff, 100, 200 / portTICK_RATE_MS);
            print_string = strstr((const char *)AT_command.AT_respose_buff, ok_string);
            ESP_LOGI(TAG1, "AT error");
        }
        if (print_string != NULL)
        {
            #ifdef DEBUG_PORT
            uart_write_bytes(UART0, (const char *)AT_command.AT_respose_buff, size);
            #endif
            print_string = NULL;
            memset(AT_command.AT_respose_buff, 0, 200);
        }
         while (print_string == NULL)
        {
            uart_write_bytes(UART1, (const char *)"AT+CEREG=4\r", 11);
            size = uart_read_bytes(UART1, AT_command.AT_respose_buff, 100, 200 / portTICK_RATE_MS);
            print_string = strstr((const char *)AT_command.AT_respose_buff, ok_string);
            ESP_LOGI(TAG1, "AT+CEREG=4 ERROR");
        }
        if(print_string != NULL)
        {
            #ifdef DEBUG_PORT
            uart_write_bytes(UART0, (const char *)AT_command.AT_respose_buff, size);
            #endif
            print_string = NULL;
            memset(AT_command.AT_respose_buff, 0, 200);
        }

    }
    else if(status == 5)
    {
        while (print_string == NULL)
        {
            uart_write_bytes(UART1, (const char *)"AT\r", 3);
            size = uart_read_bytes(UART1, AT_command.AT_respose_buff, 100, 200 / portTICK_RATE_MS);
            print_string = strstr((const char *)AT_command.AT_respose_buff, ok_string);
            ESP_LOGI(TAG1, "AT error");
        }
        if (print_string != NULL)
        {
            #ifdef DEBUG_PORT
            uart_write_bytes(UART0, (const char *)AT_command.AT_respose_buff, size);
            #endif
            print_string = NULL;
            memset(AT_command.AT_respose_buff, 0, 200);
        }
         while (print_string == NULL)
        {
            uart_write_bytes(UART1, (const char *)"AT+CEREG=5\r", 11);
            size = uart_read_bytes(UART1, AT_command.AT_respose_buff, 100, 200 / portTICK_RATE_MS);
            print_string = strstr((const char *)AT_command.AT_respose_buff, ok_string);
            ESP_LOGI(TAG1, "AT+CEREG=5 ERROR");
        }
        if(print_string != NULL)
        {
            #ifdef DEBUG_PORT
            uart_write_bytes(UART0, (const char *)AT_command.AT_respose_buff, size);
            #endif
            print_string = NULL;
            memset(AT_command.AT_respose_buff, 0, 200);
        }
    }
}


void L1_bc660_ssl_configuration(uint8_t contextID, uint8_t  connectID, uint8_t  sslversion)
{

uint8_t dest1[2]={0};
uint8_t dest2[2]={0};
uint8_t dest3[2]={0};
char *print_string = NULL;
char *ok_string = "AT+QSSLCFG=0,0,\"seclevel\",2\r\r\nOK\r\n";
uint16_t size = 0;

AT_command.AT_send_buff = malloc(40*sizeof(int));


itoa(contextID,(char *)dest1,10);
itoa(connectID,(char *)dest2,10);
itoa(sslversion,(char *)dest3,10);

strcpy((char *)AT_command.AT_send_buff,"AT+QSSLCFG=");
strcat((char *)AT_command.AT_send_buff, (char *)dest1);
strcat((char *)AT_command.AT_send_buff, ",");
strcat((char *)AT_command.AT_send_buff, (char *)dest2);
strcat((char *)AT_command.AT_send_buff, ",");
strcat((char *)AT_command.AT_send_buff, "\"seclevel\",");
strcat((char *)AT_command.AT_send_buff, (char *)dest3);
strcat((char *)AT_command.AT_send_buff, "\r");

 while (print_string == NULL)
        {
            uart_write_bytes(UART1, (const char *)AT_command.AT_send_buff, 30);  //"AT+QSSLCFG=0,0,\"seclevel\",2\r"
            vTaskDelay(100);
          if(flag == 1)
            {
            size = uart_read_bytes(UART1, AT_command.AT_respose_buff, 100, 200 / portTICK_RATE_MS);
            print_string = strstr((const char *)AT_command.AT_respose_buff, ok_string);
            ESP_LOGI(TAG1, "%s",AT_command.AT_respose_buff);
            }
        }
        if(print_string != NULL)
        {
          //  #ifdef DEBUG_PORT
           // uart_write_bytes(UART0, (const char *)AT_command.AT_respose_buff, size);
           // #endif
            print_string = NULL;
            memset(AT_command.AT_respose_buff, 0, 200);
        }

}


void L1_bc660_MQTT_configuration(uint8_t MQTT_socket, uint8_t  SSL_status, uint8_t  ssl_contextid, uint8_t ssl_connectid, uint8_t version)
{
uint8_t dest1[2]={0};
uint8_t dest2[2]={0};
uint8_t dest3[2]={0};
uint8_t dest4[2]={0};
uint8_t dest5[2]={0};

char *print_string = NULL;
char *ok_string = "OK";
uint16_t size = 0;

AT_command.AT_send_buff = malloc(40*sizeof(int));


itoa(MQTT_socket,(char *)dest1,10);
itoa(SSL_status,(char *)dest2,10);
itoa(ssl_contextid,(char *)dest3,10);
itoa(ssl_connectid,(char *)dest4,10);
itoa(version,(char *)dest5,10);

strcpy((char *)AT_command.AT_send_buff,"AT+QMTCFG=ssl,");

strcat((char *)AT_command.AT_send_buff, (char *)dest1);
strcat((char *)AT_command.AT_send_buff, ",");
strcat((char *)AT_command.AT_send_buff, (char *)dest2);
strcat((char *)AT_command.AT_send_buff, ",");
strcat((char *)AT_command.AT_send_buff, (char *)dest3);
strcat((char *)AT_command.AT_send_buff, ",");
strcat((char *)AT_command.AT_send_buff, (char *)dest4);
strcat((char *)AT_command.AT_send_buff, "\r");

        while (print_string == NULL)
        {
            vTaskDelay(100);
            uart_write_bytes(UART1, (const char *)AT_command.AT_send_buff, 22);  //"AT+QSSLCFG=0,0,\"seclevel\",2\r"
            vTaskDelay(100);
          if(flag == 1)
            {
            size = uart_read_bytes(UART1, AT_command.AT_respose_buff, 100, 200 / portTICK_RATE_MS);
            print_string = strstr((const char *)AT_command.AT_respose_buff, ok_string);
            ESP_LOGI(TAG1, "%s",AT_command.AT_respose_buff);
            memset(AT_command.AT_respose_buff, 0, 200);
            }
        }
        if(print_string != NULL)
        {
          //  #ifdef DEBUG_PORT
           // uart_write_bytes(UART0, (const char *)AT_command.AT_respose_buff, size);
           // #endif
            print_string = NULL;
            memset(AT_command.AT_respose_buff, 0, 200);
        }

        strcpy((char *)AT_command.AT_send_buff,"AT+QMTCFG=\"version\",");
        strcat((char *)AT_command.AT_send_buff, (char *)dest1);
        strcat((char *)AT_command.AT_send_buff, ",");
        strcat((char *)AT_command.AT_send_buff, (char *)dest5);
        strcat((char *)AT_command.AT_send_buff, "\r");
        

        while (print_string == NULL)
        {
            uart_write_bytes(UART1, (const char *)AT_command.AT_send_buff, 26);  //"AT+QSSLCFG=0,0,\"seclevel\",2\r"
            vTaskDelay(100);
            if(flag == 1)
            {
            size = uart_read_bytes(UART1, AT_command.AT_respose_buff, 100, 200 / portTICK_RATE_MS);
            print_string = strstr((const char *)AT_command.AT_respose_buff, ok_string);
            ESP_LOGI(TAG1, "%s",AT_command.AT_respose_buff);
            }
        }
        if(print_string != NULL)
        {
           //  #ifdef DEBUG_PORT
           // uart_write_bytes(UART0, (const char *)AT_command.AT_respose_buff, size);
           // #endif
            print_string = NULL;
            memset(AT_command.AT_respose_buff, 0, 200);
        }


}

void L1_bc660_MQTT_clienconnection(uint8_t MQTT_socket, uint8_t host_name[], uint32_t port)
{
    uint8_t dest1[2]={0};
    uint8_t dest2[5]={0};


    char *print_string = NULL;
    char *ok_string = "OK";
    uint16_t size = 0;

   AT_command.AT_send_buff = malloc(100*sizeof(int));

    itoa(MQTT_socket,(char *)dest1,10);
    itoa(port,(char *)dest2,10);
    strcpy((char *)AT_command.AT_send_buff,"AT+QMTOPEN=");
    strcat((char *)AT_command.AT_send_buff, (char *)dest1);
    strcat((char *)AT_command.AT_send_buff, ",");
    strcat((char *)AT_command.AT_send_buff, (char *)host_name);
    strcat((char *)AT_command.AT_send_buff, ",");
    strcat((char *)AT_command.AT_send_buff, (char *)dest2);
    strcat((char *)AT_command.AT_send_buff, "\r");


    while (print_string == NULL)
        {
            uart_write_bytes(UART1, (const char *)AT_command.AT_send_buff, strlen((const char *)AT_command.AT_send_buff));  //"AT+QSSLCFG=0,0,\"seclevel\",2\r"
            vTaskDelay(100);
            if(flag == 1)
            {
                flag =0;
            size = uart_read_bytes(UART1, AT_command.AT_respose_buff, 100, 200 / portTICK_RATE_MS);
            print_string = strstr((const char *)AT_command.AT_respose_buff, ok_string);
            ESP_LOGI(TAG1, "%s",AT_command.AT_respose_buff);
            memset(AT_command.AT_respose_buff, 0, 200);
            }
        }
        if(print_string != NULL)
        {
            MQTT_clientconnectionflag = 1;
            while(1)
            {
                if( 1 == flag)
                {
                flag=0;
                uart_read_bytes(UART1, AT_command.AT_respose_buff, 100, 200 / portTICK_RATE_MS);    // response check condition has to be implemented
                print_string = strstr((const char *)AT_command.AT_respose_buff, ok_string);
                ESP_LOGI(TAG1, "%s",AT_command.AT_respose_buff);
                break;
                }
            }
            print_string = NULL;
            memset(AT_command.AT_respose_buff, 0, 200);
        }

        strcpy((char *)AT_command.AT_send_buff,"AT+QMTCONN=");
        strcat((char *)AT_command.AT_send_buff, (char *)dest1);
        strcat((char *)AT_command.AT_send_buff, ",");
        strcat((char *)AT_command.AT_send_buff, "\"sense_test\"");
        strcat((char *)AT_command.AT_send_buff, "\r");

        while (print_string == NULL)
        {
            uart_write_bytes(UART1, (const char *)AT_command.AT_send_buff, strlen((const char *)AT_command.AT_send_buff));  //"AT+QSSLCFG=0,0,\"seclevel\",2\r"
            vTaskDelay(100);
            if(flag == 1)
            {
                flag =0;
            size = uart_read_bytes(UART1, AT_command.AT_respose_buff, 100, 200 / portTICK_RATE_MS);
            print_string = strstr((const char *)AT_command.AT_respose_buff, ok_string);
            ESP_LOGI(TAG1, "%s",AT_command.AT_respose_buff);
            memset(AT_command.AT_respose_buff, 0, 200);
            }
        }
        if(print_string != NULL)
        {
            
            print_string = NULL;
            memset(AT_command.AT_respose_buff, 0, 200);
        }

}

void L1_bc660_MQTT_Publish(uint8_t MQTT_socket, uint8_t msgID, uint32_t QoS, uint8_t retain, char *topic, uint32_t msg_lenght, char *msg)
{
    uint8_t dest1[2]={0};
    uint8_t dest2[2]={0};
    uint8_t dest3[2]={0};
    uint8_t dest4[2]={0};
    uint8_t dest5[2]={0};

    char *print_string = NULL;
    char *ok_string = "OK";
    uint16_t size = 0;

    AT_command.AT_send_buff = malloc(100*sizeof(int));

    itoa(MQTT_socket,(char *)dest1,10);
    itoa(msgID,(char *)dest2,10);
    itoa(QoS,(char *)dest3,10);
    itoa(retain,(char *)dest4,10);
    itoa(msg_lenght,(char *)dest5,10);

    strcpy((char *)AT_command.AT_send_buff,"AT+QMTPUB=");
    strcat((char *)AT_command.AT_send_buff, (char *)dest1);
    strcat((char *)AT_command.AT_send_buff, ",");
    strcat((char *)AT_command.AT_send_buff, (char *)dest2);
    strcat((char *)AT_command.AT_send_buff, ",");
    strcat((char *)AT_command.AT_send_buff, (char *)dest3);
    strcat((char *)AT_command.AT_send_buff, ",");
    strcat((char *)AT_command.AT_send_buff, (char *)dest4);
    strcat((char *)AT_command.AT_send_buff, ",");
    strcat((char *)AT_command.AT_send_buff, (char *)topic);
    strcat((char *)AT_command.AT_send_buff, ",");
    strcat((char *)AT_command.AT_send_buff, (char *)dest5);
    strcat((char *)AT_command.AT_send_buff, ",");
    strcat((char *)AT_command.AT_send_buff, (char *)msg);
    strcat((char *)AT_command.AT_send_buff, "\r");

     while (print_string == NULL)
        {
            uart_write_bytes(UART1, (const char *)AT_command.AT_send_buff, strlen((const char *)AT_command.AT_send_buff));  //"AT+QSSLCFG=0,0,\"seclevel\",2\r"
            vTaskDelay(100);
            if(flag == 1)
            {
                flag =0;
            size = uart_read_bytes(UART1, AT_command.AT_respose_buff, 100, 200 / portTICK_RATE_MS);
            print_string = strstr((const char *)AT_command.AT_respose_buff, ok_string);
            ESP_LOGI(TAG1, "%s",AT_command.AT_respose_buff);
            memset(AT_command.AT_respose_buff, 0, 200);
            }
        }
        if(print_string != NULL)
        {
           
            print_string = NULL;
            memset(AT_command.AT_respose_buff, 0, 200);
        }


}


