#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "semphr.h"
#include "timer_interrupts.h"

SemaphoreHandle_t xSemaphore;
	
void LedBlink(void *pvParameters) {
	while(1) {
		xSemaphoreTake(xSemaphore, portMAX_DELAY);
		LedToggle(0);
	}
}

int main(void) {
	
	LedInit();
	Timer1Interrupts_Init(50000, &xSemaphore);
	xSemaphore = xSemaphoreCreateBinary();
	
	xTaskCreate(LedBlink, NULL, 128, NULL, 1, NULL);
	vTaskStartScheduler();
	while(1) {};
}
