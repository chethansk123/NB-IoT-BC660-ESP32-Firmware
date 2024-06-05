

#include<stdint.h>
#include "L1_bc660.h"
#include "esp_log.h"
#include "esp_err.h"
#include "mcal_uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
AT_commad_t AT_command;
#define BUF_SIZE 1024

static const char *TAG = "uart_events";
static QueueHandle_t uart1_queue;
extern uint8_t flag =0;

extern uint8_t MQTT_clientconnectionflag;


static void uart_event_task(void *pvParameters)
{
    uart_event_t event;
     int len;
    size_t buffered_size;
    uint8_t *dtmp = (uint8_t *) malloc(BUF_SIZE);
    uint8_t *data = (uint8_t *) malloc(BUF_SIZE);
    while (1) {
        /* Waiting for UART event.
           If it happens then print out information what is it */
        if (xQueueReceive(uart1_queue, (void * )&event, (portTickType)portMAX_DELAY)) {
            ESP_LOGI(TAG, "uart[%d] event:", UART1);
            switch (event.type) {
            case UART_DATA:
                /* Event of UART receiving data
                 * We'd better handler data event fast, there would be much more data events
                 * than other types of events.
                 * If we take too much time on data event, the queue might be full.
                 * In this example, we don't process data in event, but read data outside.
                 */
               // uart_get_buffered_data_len(UART1, &buffered_size);
                //ESP_LOGI(TAG, "data, len: %d; buffered len: %d", event.size, buffered_size);
                flag =1;

                if(MQTT_clientconnectionflag==1)
                {
                    MQTT_clientconnectionflag =0;
                }
                // len = uart_read_bytes(UART1, AT_command.AT_respose_buff, BUF_SIZE, 100 / portTICK_RATE_MS);
               // if (len > 0) {
               // ESP_LOGI(TAG, "uart read : %d", len);
               // uart_write_bytes(UART0, (const char *)AT_command.AT_respose_buff, len);
        //}
                //uint8_t *data = (uint8_t *) malloc(BUF_SIZE);
               // uart_read_bytes(UART0, data, BUF_SIZE, 100 / portTICK_RATE_MS);
                break;
            case UART_FIFO_OVF:
                ESP_LOGE(TAG, "hw fifo overflow");
                // If fifo overflow happened, you should consider adding flow control for your application.
                // We can read data out out the buffer, or directly flush the Rx buffer.
                uart_flush(UART1);
                break;
            case UART_BUFFER_FULL:
                ESP_LOGE(TAG, "ring buffer full");
                // If buffer full happened, you should consider increasing your buffer size
                // We can read data out out the buffer, or directly flush the Rx buffer.
                uart_flush(UART1);
                break;
            case UART_BREAK:
                ESP_LOGI(TAG, "uart rx break detected");
                break;
            case UART_PARITY_ERR:
                ESP_LOGE(TAG, "uart parity error");
                break;
            case UART_FRAME_ERR:
                ESP_LOGE(TAG, "uart frame error");
                break;
            case UART_PATTERN_DET:
                ESP_LOGI(TAG, "uart pattern detected");
                break;
            default:
                ESP_LOGE(TAG, "not serviced uart event type: %d\n", event.type);
                break;
            }
        }
    }
    free(dtmp);
    dtmp = NULL;
    vTaskDelete(NULL);
}

/*****************************************************************************/
/**
* \brief This function initialize the UART.
*
* \description
* Two UART's of the ESP32 are initialized UART 0 is for debug console and UART 1
  is used for the communication between Totalizer board and esp32
* UART 0 baud rate : 115200
* UART 1 baud rate : 38400
* @param    None
*  
* @return   None
*
* @author   SK
*
* @date     02-11-2021
*
* @note
********************************************************************************/

void mcal_uartinit(void)
{
    uint8_t intr_alloc_flags = 0;

    /* UART0 */
    uart_config_t debug_uart_config = {
        .baud_rate = UART0_BAUDRATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    ESP_ERROR_CHECK(uart_driver_install(UART0, UART0_BUFF_SIZE * 2, 0, 0, NULL, intr_alloc_flags)); //&uart0_queue
    ESP_ERROR_CHECK(uart_param_config(UART0, &debug_uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART0, UART0_TX, UART0_RX, ECHO_TEST_RTS, ECHO_TEST_CTS));  

   // uart_enable_pattern_det_intr(UART0, '+', 3, 10000, 10, 10);

    // Create a task to handle uart event from ISR
    
    //ESP_ERROR_CHECK(uart_isr_free(UART0));
    //ESP_ERROR_CHECK(uart_isr_register(UART0,uart_intr_handle, NULL, ESP_INTR_FLAG_LEVEL1, &handle_console));
    //ESP_ERROR_CHECK(uart_enable_rx_intr(UART0));



   // printf("UART0 initialized");

    /* UART1 */
    uart_config_t mid_uart_config = {   
        .baud_rate = UART1_BAUDRATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

   // ESP_ERROR_CHECK(uart_driver_install(UART1, UART1_BUFF_SIZE * 2, 0, 0, NULL, intr_alloc_flags));
   // ESP_ERROR_CHECK(uart_param_config(UART1, &mid_uart_config));
   // ESP_ERROR_CHECK(uart_set_pin(UART1, UART1_TX, UART1_RX, ECHO_TEST_RTS, ECHO_TEST_CTS));  //10,9



    ESP_ERROR_CHECK(uart_driver_install(UART1, UART1_BUFF_SIZE * 2, 0, 10, &uart1_queue, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(UART1, &mid_uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART1, UART1_TX, UART1_RX, ECHO_TEST_RTS, ECHO_TEST_CTS));  //10,9
   
    xTaskCreate(uart_event_task, "uart_event_task", 2048, NULL, 12, NULL);
    
    #if CONFIG_UART_ISR_IN_IRAM
    intr_alloc_flags = ESP_INTR_FLAG_IRAM;
    #endif


}
