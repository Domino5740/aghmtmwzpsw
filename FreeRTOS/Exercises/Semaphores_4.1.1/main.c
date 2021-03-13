#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "semphr.h"

struct LedParam {
	unsigned char ucBlinkingFreq;
	unsigned char ucLedIndex;
};

void LedBlink( void *pvParameters ){
	struct LedParam *sLedParam = (struct LedParam*)pvParameters;
	while(1){		
		LedToggle(sLedParam -> ucLedIndex);
		vTaskDelay((1000/(sLedParam -> ucBlinkingFreq)) / 2);
	}
}

void LedCtrl(void *pvParameters) {
	struct LedParam *sLedParam = (struct LedParam*)pvParameters;
	unsigned char uc2divider = 0;
	while(1) {
		vTaskDelay(1000);
		(sLedParam -> ucBlinkingFreq) = ((sLedParam -> ucBlinkingFreq) + 1) % 25;
		if(uc2divider % 2) {
			(sLedParam -> ucLedIndex) = ((sLedParam -> ucLedIndex) + 1) % 4;
		}
		uc2divider++;
	}
}

void TaskSuspend(void *pvParameters) {
	xTaskHandle *xBlinkHandle = (xTaskHandle*)pvParameters;
	while(1) {
		vTaskDelay(1000);
		vTaskSuspend(*xBlinkHandle);
		vTaskDelay(1000);
		vTaskResume(*xBlinkHandle);
	}
}

void PulseTrigger(void *pvParameters) {
	xSemaphoreHandle *xSemaphore = (xSemaphoreHandle*)pvParameters;
	while(1) {
		xSemaphoreGive(*xSemaphore);
		vTaskDelay(1000);
	}
}

void PulseTrigger_03(void *pvParameters) {
	xSemaphoreHandle *xSemaphore = (xSemaphoreHandle*)pvParameters;
	vTaskDelay(1000/3);
	while(1) {
		xSemaphoreGive(*xSemaphore);
		vTaskDelay(1000/3);
	}
}

void Pulse_LED0(void *pvParameters) {
	xSemaphoreHandle *xSemaphore = (xSemaphoreHandle*)pvParameters;
	while(1) {
		xSemaphoreTake(*xSemaphore, portMAX_DELAY);
		LedOn(0);
		vTaskDelay(100);
		LedOff(0);
	}
}

void Pulse_LED1(void *pvParameters) {
	xSemaphoreHandle *xSemaphore = (xSemaphoreHandle*)pvParameters;
	while(1) {
		xSemaphoreTake(*xSemaphore, portMAX_DELAY);
		LedOn(1);
		vTaskDelay(100);
		LedOff(1);
	}
}


int main(void) {
	xSemaphoreHandle xSemaphore;
	vSemaphoreCreateBinary(xSemaphore);
	
	LedInit();
	
	xTaskCreate(PulseTrigger, NULL , 100 , &xSemaphore, 2 , NULL);
	//xTaskCreate(PulseTrigger_03, NULL , 100 , &xSemaphore, 2 , NULL);
	xTaskCreate(Pulse_LED0, NULL , 100 , &xSemaphore, 2 , NULL);
	xTaskCreate(Pulse_LED1, NULL , 100 , &xSemaphore, 2 , NULL);
	vTaskStartScheduler();
	while(1);
}
