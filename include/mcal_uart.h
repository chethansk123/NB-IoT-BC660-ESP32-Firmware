#ifndef __MCAL_UART_H__
#define __MCAL_UART_H__
#include "driver/uart.h"

#define UART0_BAUDRATE 115200
#define UART1_BAUDRATE 115200

#define ECHO_TEST_RTS (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS (UART_PIN_NO_CHANGE)

typedef enum 
{
    UART0=0,
    UART1,

}t_uart_port_num;

typedef enum 
{
    UART0_TX = 1,
    UART0_RX =3,
    UART0_BUFF_SIZE=1024,
    UART0_TEST_RTS = -1,
    UART0_TEST_CTS = -1,

}t_uart0_pin_config;


typedef enum 
{
    UART1_TX = 19,
    UART1_RX =18,
    UART1_BUFF_SIZE=1024,
    UART1_TEST_RTS = -1,
    UART1_TEST_CTS = -1,


}t_uart1_pin_config;

void mcal_uartinit(void);

#endif