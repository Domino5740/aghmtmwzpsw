#include <LPC21xx.H>
#include "FreeRTOS.h"
#include "task.h"
#include "keyboard.h"
#include "servo.h"

#define S0_bm (1<<4)
#define S1_bm (1<<6)
#define S2_bm (1<<5)
#define S3_bm (1<<7)

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

enum eKeyboardState eKeyboardRead() {

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

void KeyboardInit() {
	IO0DIR = (IO0DIR & ~(S0_bm | S1_bm | S2_bm | S3_bm));
	xTaskCreate(Keyboard, NULL, 128, NULL, 1, NULL );
}
