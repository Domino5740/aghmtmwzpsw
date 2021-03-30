__irq void Timer1IRQHandler(void);
void Timer1Interrupts_Init(unsigned int uiPeriod, void (*ptrInterruptFunction)());
void WatchUpdate(void);

struct Watch {
	unsigned char ucMinutes;
	unsigned char ucSeconds;
	unsigned char fSecondsValueChanged;
	unsigned char fMinutesValueChanged;
};
