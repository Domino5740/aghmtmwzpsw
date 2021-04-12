#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "keyboard.h"
#include "watch.h"
//ex1
void KeyboardLedToggle( void *pvParameters ) {

	enum KeyboardState eKeyboardState;
	
	while(1){
		eKeyboardState = eKeyboardRead();
		
		switch(eKeyboardState) {
			case BUTTON_0:
				LedToggle(0);
			break;
			case BUTTON_1:
				LedToggle(1);
			break;
			case BUTTON_2:
				LedToggle(2);
			break;
			case BUTTON_3: 
				LedToggle(3);
			break;
			case RELEASED:
			break;
		}
		
		vTaskDelay(10);
	}
}
//ex2
void WatchLedToggle(void *pvParameters) {
	
	struct WatchEvent sWatchEvent;
	
	while(1) {
		
		sWatchEvent = sWatchRead();
		if(sWatchEvent.eTimeUnit == SECONDS) {
			LedToggle(1);
		}
		else if(sWatchEvent.eTimeUnit == MINUTES) {
			LedToggle(2);
		}
		vTaskDelay(1000);
	}
}

int main( void ) {
	LedInit();
	KeyboardInit();
	WatchInit();
	xTaskCreate( KeyboardLedToggle, NULL , 100 , NULL, 1 , NULL );
	xTaskCreate( WatchLedToggle, NULL , 100 , NULL, 1 , NULL );
	vTaskStartScheduler();
	while(1);
}
