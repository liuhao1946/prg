

#ifndef _SIM_UART_
#define _SIM_UART_


#include "stm8s.h"


#define SIM_UART_IO_H   GPIOA->ODR |= 0x08 
#define SIM_UART_IO_L   GPIOA->ODR &= ~0x08



void app_sim_uart_transmit(void );

void app_sim_uart_tx_set_data(unsigned char  data);

unsigned char app_sim_uart_busy(void );

void app_sim_uart_init(void );

#endif