enum DetectorState {
	ACTIVE, INACTIVE
};

enum ServoState {CALLIB, IDLE, IN_PROGRESS};
struct Servo {
	enum ServoState eState;
	unsigned int uiCurrentPosition;
	unsigned int uiDesiredPosition;
};

void DetectorInit(void);
enum DetectorState eReadDetector(void);
void ServoInit(unsigned int uiServoFrequency);
void ServoCallib(void);
void ServoGoTo(unsigned int uiPosition);
void ServoAutomat(void);
