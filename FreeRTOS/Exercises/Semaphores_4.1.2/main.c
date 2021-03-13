#include "uart.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "keyboard.h"

void Rtos_Transmitter_SendString(char cString[], xSemaphoreHandle *xSemaphore) {
	xSemaphoreTake(*xSemaphore, portMAX_DELAY);
	Transmitter_SendString(cString);
	while (Transmitter_GetStatus()!=FREE){};
	xSemaphoreGive(*xSemaphore);
}

void LettersTx (void *pvParameters){
	xSemaphoreHandle *xSemaphore = (xSemaphoreHandle*)pvParameters;
	char cHexStr[20];
	char cStringToSend[50];
	while(1){
		UIntToHexStr(xTaskGetTickCount(), cHexStr);
		CopyString("-ABCDEEFGH-:", cStringToSend);
		AppendString(cHexStr, cStringToSend);
		AppendString("\n", cStringToSend);
		Rtos_Transmitter_SendString(cStringToSend, xSemaphore);
		vTaskDelay(300);
	}
}

void KeyboardTx(void *pvParameters) {
	xSemaphoreHandle *xSemaphore = (xSemaphoreHandle*)pvParameters;
	while(1) {
		if(eKeyboardRead() != RELEASED) {
			Rtos_Transmitter_SendString("-Keyboard-\n", xSemaphore);
		}
	}
}

int main(void){
	xSemaphoreHandle xSemaphore;
	vSemaphoreCreateBinary(xSemaphore);
	
	KeyboardInit();
	UART_InitWithInt(300);
	
	xTaskCreate(LettersTx, NULL, 128, &xSemaphore, 1, NULL );
	xTaskCreate(KeyboardTx, NULL, 128, &xSemaphore, 1, NULL );
	vTaskStartScheduler();
	while(1);
}
