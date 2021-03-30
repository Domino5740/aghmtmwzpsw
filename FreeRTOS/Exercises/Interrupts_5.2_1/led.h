void LedInit(void);
void LedStepRight(void);
void LedStepLeft(void);	
void LedOn(unsigned char ucLedIndex);
void LedOff(unsigned char ucLedIndex);
void LedToggle(unsigned char ucLedIndex);

enum eDirection {
	LEFT,
	RIGHT
};

enum LedState {
	LED_STEP_LEFT,
	LED_NO_STEP,
	LED_STEP_RIGHT
};
