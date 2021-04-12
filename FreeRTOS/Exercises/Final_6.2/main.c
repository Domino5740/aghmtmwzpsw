#include "FreeRTOS.h"
#include "task.h"
#include "keyboard.h"
#include "uart.h"
#include "string.h"
#include "queue.h"
#include "command_decoder.h"
#include "servo.h"

xQueueHandle xEventQueue;
extern struct Token asToken[MAX_TOKEN_NR];
extern unsigned char ucTokenNr;

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
			xQueueSendToBack(xEventQueue, &cStringToBeSend, portMAX_DELAY);
		}
		vTaskDelay(10);
	}
}

void UartRx(void *pvParameters) {
	
	char cReceivedString[UART_RX_BUFFER_SIZE] = "\0";
	
	while(1) {
		Uart_GetString(cReceivedString);
		xQueueSendToBack(xEventQueue, &cReceivedString, portMAX_DELAY);
		vTaskDelay(10);
	}
}

void Executor(void *pvParameters) {
	
	char cReceivedEvent[20] = "\0";
	
	while(1) {
		if(xQueueReceive(xEventQueue, &cReceivedEvent, 0) == pdTRUE) {
			
			DecodeMsg(cReceivedEvent);
			
			if(asToken[0].eType == KEYWORD && ucTokenNr != 0) {
				switch(asToken[0].uValue.eKeyword) {
					case ID:
						Uart_PutString("id LPC2132");
						break;
					case CALIB:
						ServoCallib();
						Uart_PutString("ok");
						break;
					case GOTO:
						if(asToken[1].eType == NUMBER) {
							ServoGoTo(asToken[1].uValue.uiNumber);
							Uart_PutString("ok");
						}
						else {
							Uart_PutString("error, no number");
						}
						break;
					case BUTTON:
						switch(asToken[1].uValue.uiNumber) {
							case 0:
								ServoCallib();
								break;
							case 1:
								ServoGoTo(10);
								break;
							case 2:
								ServoGoTo(20);
								break;
							case 3:
								ServoGoTo(40);
								break;
							default:
								break;
						}
						Uart_PutString("ok");
						break;
				}
			}
		}
		vTaskDelay(10);
	}
	
		
}

int main( void ) {
	
	unsigned int ucServoFreq = 200;
	
	KeyboardInit();
	UART_InitWithInt(9600);
	ServoInit(&ucServoFreq);
	
	xEventQueue = xQueueCreate(20, sizeof(char[20]));
	xTaskCreate( KeyboardThread, NULL , 100 , NULL, 1 , NULL );
	xTaskCreate( UartRx, NULL , 100 , NULL, 1 , NULL );
	xTaskCreate( Executor, NULL , 100 , NULL, 1 , NULL );
	vTaskStartScheduler();
	while(1);
}
