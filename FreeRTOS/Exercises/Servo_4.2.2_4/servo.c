#include "FreeRTOS.h"
#include "servo.h"
#include "queue.h"
#include "led.h"
#include "task.h"
#include <LPC21xx.H>

#define DETECTOR_bm (1<<10)

xQueueHandle xQueue;

void ServoAutomat(void *pvParameters) {
	
	struct Servo sServo;
	struct Servo sServoReceived;
	unsigned int uiDelay = (1000 / *(unsigned int*)pvParameters);
	
	sServo.eState = IDLE;
	
	while(1) {
		switch(sServo.eState) {
			case IN_PROGRESS:
				if(sServo.uiCurrentPosition < sServo.uiDesiredPosition) {
					LedStepRight();
					sServo.uiCurrentPosition++;
				}
				else if(sServo.uiCurrentPosition > sServo.uiDesiredPosition){
					LedStepLeft();
					sServo.uiCurrentPosition--;
				}
				else {
					sServo.eState = IDLE;
				}
			break;
			case IDLE:
				if(xQueueReceive(xQueue, &sServoReceived, 100) == pdTRUE) {
					sServo.uiDesiredPosition = sServoReceived.uiDesiredPosition;
					sServo.eState = sServoReceived.eState;
				}
				else if(sServo.uiCurrentPosition != sServo.uiDesiredPosition) {
					sServo.eState = IN_PROGRESS;
				}
				else {}
			break;
			case CALLIB:
				if(eReadDetector() == ACTIVE) {
					sServo.eState = IDLE;
					sServo.uiCurrentPosition = 0;
					sServo.uiDesiredPosition = 0;
				}
				else {
					LedStepLeft();
				}
			break;
		}
		vTaskDelay(uiDelay);
	}
}

void DetectorInit() {
	IO0DIR = IO0DIR & (~DETECTOR_bm);
}

enum DetectorState eReadDetector() {
	if ((IO0PIN & DETECTOR_bm) == 0) {
		return ACTIVE;
	}
	else {
		return INACTIVE;
	}
}
/*4 takty zegara na obsluge 32b uint(zwykle uint to 32b, ale zalezy pewnie od kompilatora)
moze nastapic wywlaszczenie i przejscie do innego watku podczas pracy na zmiennej wspóldzielonej(globalnej)
w wyniku czego np otrzymujemy bzdurna wartosc, gdy inny watek podmieni nam uiDesiredPosition na inna*/

void ServoInit(unsigned int *uiServoFrequency) {
	xQueue = xQueueCreate(10, sizeof(struct Servo));
	
	LedInit();
	DetectorInit();
	xTaskCreate(ServoAutomat, NULL, 128, uiServoFrequency, 1, NULL);
	ServoCallib();
}

void ServoCallib() {
	struct Servo sServo;
	
	sServo.eState = CALLIB;
	xQueueSendToBack(xQueue, &sServo, 100);
}

void ServoGoTo(unsigned int uiPosition) {
	struct Servo sServo;
	
	sServo.uiDesiredPosition = uiPosition;
	sServo.eState = IN_PROGRESS;
	xQueueSendToBack(xQueue, &sServo, 100);
}
