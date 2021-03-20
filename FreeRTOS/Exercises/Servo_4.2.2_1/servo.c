#include "servo.h"
#include "led.h"
#include "timer_interrupts.h"
#include <LPC21xx.H>

#define DETECTOR_bm (1<<10)

struct Servo sServo;

void ServoAutomat() {
	DetectorInit();
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
			if(sServo.uiCurrentPosition != sServo.uiDesiredPosition) {
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

void ServoInit(unsigned int uiServoFrequency) {
	LedInit();
	sServo.eState = CALLIB;
	Timer1Interrupts_Init((1000000/uiServoFrequency), &ServoAutomat);
}

void ServoCallib() {
	sServo.eState = CALLIB;
}

void ServoGoTo(unsigned int uiPosition) {
	sServo.uiDesiredPosition = uiPosition;
}
