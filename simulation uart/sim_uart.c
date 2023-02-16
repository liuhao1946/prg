		
#include "sim_uart.h"
		
		
struct
{
	unsigned char data;
	unsigned char step : 3,
				  busy_f : 1,
				  bit_cnt: 4;		  
}sim_uart;
		
typedef enum
{
  SIM_UART_IDEL,
  SIM_UART_DATA_SEND_START,
  SIM_UART_DATA_SEND,
  SIM_UART_DATA_SEND_DONE,
  SIM_UART_DATA_SEND_INIT,
}sim_uart_send_state_t;
		
////////////////////////////////////////////////////////////////////////////////

void app_sim_uart_init(void )
{
	sim_uart.step = SIM_UART_DATA_SEND_INIT;
	sim_uart.busy_f  = 0;
	SIM_UART_IO_H; 
}   

unsigned char app_sim_uart_busy(void )
{
	if(!sim_uart.busy_f)
	{
		return 0;
	}
  
	return 1;
}

void  app_sim_uart_tx_set_data(unsigned char data)
{
	sim_uart.data = data;
	sim_uart.busy_f = 1;
	sim_uart.step = SIM_UART_DATA_SEND_INIT;
}

void app_sim_uart_transmit(void )
{
	if(sim_uart.busy_f == 1)
	{	  
		switch(sim_uart.step)
		{
			case SIM_UART_DATA_SEND:
				 if(sim_uart.data & 1) 
				 {
				   SIM_UART_IO_H;
				 }
				 else 
				 {
				   SIM_UART_IO_L;
				 }
					
				 sim_uart.data >>= 1;
				 if(++sim_uart.bit_cnt >= 8)
				 {
					sim_uart.bit_cnt = 0;
					sim_uart.step = SIM_UART_DATA_SEND_DONE;
				 }
				 break;
					
			case SIM_UART_DATA_SEND_DONE:
				 SIM_UART_IO_H;
				 sim_uart.start_f = 0;
				 sim_uart.step = SIM_UART_DATA_SEND_INIT;
				 break;
					 
				case SIM_UART_DATA_SEND_START:
				 SIM_UART_IO_L;
				 sim_uart.step = SIM_UART_DATA_SEND;
				 break;

			case SIM_UART_DATA_SEND_INIT:
				 SIM_UART_IO_H;
				 sim_uart.bit_cnt = 0;
				 sim_uart.step = SIM_UART_DATA_SEND_START;
				 break;
		}

	}
}
	

	
	
	
	
	
