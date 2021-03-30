#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "semphr.h"
#include "timer_interrupts.h"

SemaphoreHandle_t xSemaphore;
	
void LedBlink(void) {
	xSemaphoreTake(xSemaphore, portMAX_DELAY);
	LedToggle(0);
	xSemaphoreGive(xSemaphore);
}

int main(void) {
	
	LedInit();
	Timer1Interrupts_Init(50000, &LedBlink);
	xSemaphore = xSemaphoreCreateBinary();
	
	vTaskStartScheduler();
	while(1) {};
}
