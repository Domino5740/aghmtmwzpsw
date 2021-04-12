enum DetectorState {
	ACTIVE, INACTIVE
};

enum ServoState {CALLIB, IDLE, IN_PROGRESS, WAIT, STEP_DELAY};

struct Servo {
	enum ServoState eState;
	unsigned int uiCurrentPosition;
	unsigned int uiDesiredPosition;
	unsigned int uiServoWaitTicks;
	unsigned int uiServoStepDelay;
};

void DetectorInit(void);
enum DetectorState eReadDetector(void);
void ServoInit(unsigned int *uiServoFrequency);
void ServoCallib(void);
void ServoGoTo(unsigned int uiPosition);
void ServoAutomat(void *pvParameters);
void Servo_Wait(unsigned int uiServoWaitTicks);
void Servo_Speed(unsigned int uiServoStepDelay);
struct Servo Servo_State(void);
