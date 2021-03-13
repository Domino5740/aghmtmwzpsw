#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
void Delay(unsigned int uiMiliSec) {
	unsigned int uiLoopCtr, uiDelayLoopCount;
	uiDelayLoopCount = uiMiliSec*12000;
	for(uiLoopCtr=0;uiLoopCtr<uiDelayLoopCount;uiLoopCtr++) {}
}

void Led0Blink( void *pvParameters ){
	while(1){
		LedToggle(0);
		Delay(1000);
	}
}

void Led1Blink( void *pvParameters ){
	while(1){
		LedToggle(1);
		Delay(1000);
	}
}

int main(void){
	LedInit();
	xTaskCreate(Led0Blink, NULL , 100 , NULL, 2 , NULL );
	xTaskCreate(Led1Blink, NULL , 100 , NULL, 2 , NULL ); 
	
	vTaskStartScheduler();
	//2_2: ostatni utworzony watek ma wyzszy priorytet lub jest odpalany jako 1, vTaskDelay dziala elegancko
	//2_3: higher priority task preempts lower priority task
	while(1);
}
