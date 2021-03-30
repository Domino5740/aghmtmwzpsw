#include "FreeRTOS.h"
#include "task.h"
#include "keyboard.h"
#include "servo.h"

int main(void) {
	
	unsigned int uiServoFrequency = 200;
	
	KeyboardInit();
	ServoInit(&uiServoFrequency);
	vTaskStartScheduler();
	
	while(1);
}
