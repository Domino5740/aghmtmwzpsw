#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "uart.h"
#include <string.h>

void UartRx( void *pvParameters ) {
	
	char acBuffer[UART_RX_BUFFER_SIZE];	
	
	while(1){
		Uart_GetString(acBuffer);
		if(!strcmp(acBuffer, "zero")) {
			LedToggle(0);
		}
		else if(!strcmp(acBuffer, "jeden")) {
			LedToggle(1);
		}
	}
}

int main( void ) {
	LedInit();
	UART_InitWithInt(9600);
	xTaskCreate( UartRx, NULL , 100 , NULL, 1 , NULL );
	vTaskStartScheduler();
	while(1);
}
