#include "FreeRTOS.h"
#include "task.h"
#include "keyboard.h"
#include "watch.h"
#include "uart.h"
#include "string.h"

void KeyboardThread( void *pvParameters ) {

	enum KeyboardState eKeyboardState = RELEASED;
	char cStringToBeSend[UART_TX_BUFFER_SIZE] = "button ", cHexStr[UART_TX_BUFFER_SIZE] = "0x0000";
	
	while(1) {
		eKeyboardState = eKeyboardRead();
		CopyString("button ", cStringToBeSend);
		
		switch(eKeyboardState) {
			case BUTTON_0:
				UIntToHexStr(0, cHexStr);
			break;
			case BUTTON_1:
				UIntToHexStr(1, cHexStr);
			break;
			case BUTTON_2:
				UIntToHexStr(2, cHexStr);
			break;
			case BUTTON_3: 
				UIntToHexStr(3, cHexStr);
			break;
			case RELEASED:
			break;
		}
		
		if(eKeyboardState != RELEASED) {
			AppendString(cHexStr, cStringToBeSend);
			Uart_PutString(cStringToBeSend);
		}
		vTaskDelay(100);
	}
}

void WatchThread(void *pvParameters) {
	
	struct WatchEvent sWatchEvent;
	char cStringToBeSend[UART_TX_BUFFER_SIZE] = "sek", cHexStr[UART_TX_BUFFER_SIZE] = "0x0000";
	
	while(1) {
		
		sWatchEvent = sWatchRead();
		UIntToHexStr(sWatchEvent.TimeValue, cHexStr);
		
		if(sWatchEvent.eTimeUnit == SECONDS) {
			CopyString("sek ", cStringToBeSend);
		}
		else if(sWatchEvent.eTimeUnit == MINUTES) {
			CopyString("min ", cStringToBeSend);
		}
		AppendString(cHexStr, cStringToBeSend);
		Uart_PutString(cStringToBeSend);
		vTaskDelay(1000);
	}
}

int main( void ) {
	KeyboardInit();
	WatchInit();
	UART_InitWithInt(9600);
	xTaskCreate( KeyboardThread, NULL , 100 , NULL, 1 , NULL );
	xTaskCreate( WatchThread, NULL , 100 , NULL, 1 , NULL );
	vTaskStartScheduler();
	while(1);
}
