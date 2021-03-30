#include "FreeRTOS.h"
#include "task.h"
#include "uart.h"

void UartRx( void *pvParameters ) {
	
	char acBuffer[UART_TX_BUFFER_SIZE] = "0123456789\n";

	while(1){
		vTaskDelay(500);
		Uart_PutString(acBuffer);
		//vTaskDelay(10);
		Uart_PutString(acBuffer);
		//vTaskDelay(10);
		Uart_PutString(acBuffer);
	}
}

int main( void ) {
	UART_InitWithInt(9600);
	xTaskCreate( UartRx, NULL , 100 , NULL, 1 , NULL );
	vTaskStartScheduler();
	while(1);
}
