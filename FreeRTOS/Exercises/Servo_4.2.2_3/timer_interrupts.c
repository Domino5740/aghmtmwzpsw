#include <LPC21xx.H>
#include "timer_interrupts.h"

#define mCOUNTER_ENABLE 1
#define mCOUNTER_RESET  (1<<1)

#define mINTERRUPT_ON_MR0 1
#define mRESET_ON_MR0     (1<<1)
#define mMR0_INTERRUPT    1

#define VIC_TIMER1_CHANNEL_NR 5

#define mIRQ_SLOT_ENABLE (1<<5)

#define PCLK_CLOCK 15

struct Watch sWatch;

void (*ptrTimer1InterruptFunction)();

__irq void Timer1IRQHandler(){

	T1IR=mMR0_INTERRUPT;
	ptrTimer1InterruptFunction();
	VICVectAddr=0x00;
}

void Timer1Interrupts_Init(unsigned int uiPeriod, void (*ptrInterruptFunction)()){

	ptrTimer1InterruptFunction = ptrInterruptFunction;

	VICIntEnable |= (1 << VIC_TIMER1_CHANNEL_NR);
	VICVectCntl1  = mIRQ_SLOT_ENABLE | VIC_TIMER1_CHANNEL_NR;
	VICVectAddr1  =(unsigned long)Timer1IRQHandler;

	T1MR0 = PCLK_CLOCK * uiPeriod;
	T1MCR |= (mINTERRUPT_ON_MR0 | mRESET_ON_MR0); 

	T1TCR |=  mCOUNTER_ENABLE;

}

void WatchUpdate(void) {
	if(sWatch.ucSeconds < 59) {
		sWatch.ucSeconds++;
		sWatch.fSecondsValueChanged = 1;
	}
	else {
		sWatch.ucSeconds = 0;
		sWatch.ucMinutes++;
		sWatch.fSecondsValueChanged = 1;
		sWatch.fMinutesValueChanged = 1;
	}
}
