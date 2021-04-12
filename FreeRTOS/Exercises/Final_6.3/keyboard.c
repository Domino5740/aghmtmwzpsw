#include <LPC21xx.H>
#include "FreeRTOS.h"
#include "task.h"
#include "keyboard.h"
#include "queue.h"
#include "servo.h"

#define S0_bm (1<<4)
#define S1_bm (1<<6)
#define S2_bm (1<<5)
#define S3_bm (1<<7)

/****************global variables**************/

xQueueHandle xKeyboardQueue;

/*******************threads*******************/
void Keyboard_Thread (void *pvParameters) {
	
	enum KeyboardState eKeyboardLastState;
	
	while(1){
		eKeyboardLastState = eReadButtons();
		switch(eKeyboardLastState){
			case RELEASED:
			break;
			default:
				while(eReadButtons() != RELEASED) {
					vTaskDelay(20);
				}
			break;
		}
		xQueueSendToBack(xKeyboardQueue, &eKeyboardLastState, portMAX_DELAY);
		vTaskDelay(20);
	}
}

/*******************functions*******************/

enum KeyboardState eReadButtons() {

	if((IO0PIN & S0_bm) == 0) {
		return BUTTON_0;
	}
	else if ((IO0PIN & S1_bm)== 0) {
		return BUTTON_1;
	}
	else if ((IO0PIN & S2_bm) == 0) {
		return BUTTON_2;
	}
	else if ((IO0PIN & S3_bm) == 0) {
		return BUTTON_3;
	}
	return RELEASED;
}

enum KeyboardState eKeyboardRead(void) {
	
	enum KeyboardState eKeyboardState;
	
	if(xQueueReceive(xKeyboardQueue, &eKeyboardState, 0) == pdFALSE) {
		eKeyboardState = RELEASED;
	}
	
	return eKeyboardState;
}

void KeyboardInit() {
	IO0DIR = (IO0DIR & ~(S0_bm | S1_bm | S2_bm | S3_bm));
	xKeyboardQueue = xQueueCreate(1, sizeof(enum KeyboardState));
	xTaskCreate(Keyboard_Thread, NULL, 128, NULL, 1, NULL );
}
