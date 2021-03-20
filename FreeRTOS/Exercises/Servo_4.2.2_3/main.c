#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "keyboard.h"
#include "servo.h"
#include "led.h"
#include "string.h"

void Keyboard (void *pvParameters){
	
	enum eKeyboardState eKeyboardLastState;
	
	while(1){
		eKeyboardLastState = eKeyboardRead();
		switch(eKeyboardLastState){
			case RELEASED:
			break;
			default:
				while(eKeyboardRead() != RELEASED) {}
				switch(eKeyboardLastState) {
					case BUTTON_0: ServoCallib();
					break;
					case BUTTON_1: ServoGoTo(50);
					break;
					case BUTTON_2: ServoGoTo(100);
					break;
					case BUTTON_3: ServoGoTo(150);
					break;
					case RELEASED:
					break;
				}
			break;
		}
		vTaskDelay(10);
	}
}

int main(void){
	
	unsigned int uiServoFrequency = 200;
	
	KeyboardInit();
	ServoInit(&uiServoFrequency);
	
	xTaskCreate(Keyboard, NULL, 128, NULL, 1, NULL );
	vTaskStartScheduler();
	while(1);
}
