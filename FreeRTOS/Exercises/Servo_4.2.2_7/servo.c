#include "FreeRTOS.h"
#include "servo.h"
#include "queue.h"
#include "led.h"
#include "task.h"
#include <LPC21xx.H>

#define DETECTOR_bm (1<<10)

xQueueHandle xsServoQueue;

void ServoAutomat(void *pvParameters) {
	
	struct Servo sServo;
	struct Servo sServoReceived;
	unsigned int uiDelay = (1000 / *(unsigned int*)pvParameters);
	
	sServo.eState = IDLE;
	sServo.uiServoStepDelay = 0;
	sServo.uiServoWaitTicks = 0;
	
	while(1) {
		switch(sServo.eState) {
			case IN_PROGRESS:
				sServo.eState = STEP_DELAY;
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
				if(xQueueReceive(xsServoQueue, &sServoReceived, portMAX_DELAY) == pdTRUE) {
					sServo.eState = sServoReceived.eState;
					if(sServoReceived.eState == STEP_DELAY) {
						sServo.uiServoStepDelay = sServoReceived.uiServoStepDelay;
					}
					else if(sServoReceived.eState == IN_PROGRESS) {
						sServo.uiDesiredPosition = sServoReceived.uiDesiredPosition;
					}
					else if(sServoReceived.eState == WAIT) {
						sServo.uiServoWaitTicks = sServoReceived.uiServoWaitTicks;
					}
				}
			break;
			case WAIT:
				vTaskDelay(sServo.uiServoWaitTicks);
				sServo.eState = IDLE;
			break;
			case STEP_DELAY:
				vTaskDelay(sServo.uiServoStepDelay);
				sServo.eState = IN_PROGRESS;
			break;
			case CALLIB:
				if(eReadDetector() == ACTIVE) {
					sServo.eState = IDLE;
					sServo.uiCurrentPosition = 0;
					sServo.uiDesiredPosition = 0;
					sServo.uiServoStepDelay = 0;
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
	xsServoQueue = xQueueCreate(100, sizeof(struct Servo));
	
	LedInit();
	DetectorInit();
	ServoCallib();
	xTaskCreate(ServoAutomat, NULL, 128, uiServoFrequency, 1, NULL);
}

void ServoCallib() {
	struct Servo sServoToSend;
	
	sServoToSend.eState = CALLIB;
	xQueueSendToBack(xsServoQueue, &sServoToSend, 0);
}

void ServoGoTo(unsigned int uiPosition) {
	struct Servo sServoToSend;
	
	sServoToSend.uiDesiredPosition = uiPosition;
	sServoToSend.eState = IN_PROGRESS;
	xQueueSendToBack(xsServoQueue, &sServoToSend, 0);
}

void Servo_Wait(unsigned int uiServoWaitTicks) {

	struct Servo sServoToSend;
	
	sServoToSend.eState = WAIT;
	sServoToSend.uiServoWaitTicks = uiServoWaitTicks;
	
	xQueueSendToBack(xsServoQueue, &sServoToSend, 0);
}

void Servo_Speed(unsigned int uiServoStepDelay) {
	
	struct Servo sServoToSend;
	
	sServoToSend.eState = STEP_DELAY;
	sServoToSend.uiServoStepDelay = uiServoStepDelay;
	
	xQueueSendToBack(xsServoQueue, &sServoToSend, 0);
}
