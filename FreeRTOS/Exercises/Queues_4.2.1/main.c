#include "uart.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "keyboard.h"
#include "led.h"

void Rtos_Transmitter_SendString(void *pvParameters) {
	xQueueHandle *xQueue = (xQueueHandle*)pvParameters;
	char cStringToSend[20];
	//unsigned char ucTest;
	while(1) {
		//ucTest = uxQueueMessagesWaiting(*xQueue);
		if(uxQueueMessagesWaiting(*xQueue) != 0) {
			xQueueReceive(*xQueue, cStringToSend, 100);
			Transmitter_SendString(cStringToSend);
			while (Transmitter_GetStatus()!=FREE){};
		}
	}
}

void LettersTx (void *pvParameters){
	xQueueHandle *xQueue = (xQueueHandle*)pvParameters;
	char cHexStr[20];
	char cStringToSend[20];
	unsigned char ucTest2;
	unsigned char ucTest3;
	while(1){
		CopyString("-ABCDEEFGH-:", cStringToSend);
		UIntToHexStr(xTaskGetTickCount(), cHexStr);
		AppendString(cHexStr, cStringToSend);
		AppendString("\n", cStringToSend);
		ucTest3 = xTaskGetTickCount();
		ucTest2 = xQueueSendToBack(*xQueue, cStringToSend, 100);
		ucTest3 = xTaskGetTickCount(); //0 ticks to send to queue???
		if(ucTest2 == errQUEUE_FULL) {
			LedToggle(0);
		}
		vTaskDelay(300); //przy 300 delay uart nie nadaza wysylac - 300 bps, 30 chars for sec; wysylamy wiecej
	}
}

void KeyboardTx(void *pvParameters) {
	xQueueHandle *xQueue = (xQueueHandle*)pvParameters;
	while(1) {
		if(eKeyboardRead() != RELEASED) {
			xQueueSendToBack(*xQueue, "-Keyboard-\n", portMAX_DELAY);
		}
		vTaskDelay(50);
	}
}

int main(void){
	xQueueHandle xQueue = xQueueCreate(5, 20);
	
	KeyboardInit();
	LedInit();
	UART_InitWithInt(300);
	
	xTaskCreate(LettersTx, NULL, 128, &xQueue, 1, NULL );
	xTaskCreate(KeyboardTx, NULL, 128, &xQueue, 1, NULL );
	xTaskCreate(Rtos_Transmitter_SendString, NULL, 128, &xQueue, 1, NULL );
	vTaskStartScheduler();
	while(1);
}
