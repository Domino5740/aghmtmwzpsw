#include <LPC21xx.H>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timer_interrupts.h"

#define mCOUNTER_ENABLE 1
#define mCOUNTER_RESET  (1<<1)

#define mINTERRUPT_ON_MR0 1
#define mRESET_ON_MR0     (1<<1)
#define mMR0_INTERRUPT    1

#define VIC_TIMER1_CHANNEL_NR 5

#define mIRQ_SLOT_ENABLE (1<<5)

#define PCLK_CLOCK 15

SemaphoreHandle_t *pxSemaphore;


__irq void Timer1IRQHandler(void){

	T1IR=mMR0_INTERRUPT;
	VICVectAddr=0x00;
	xSemaphoreGiveFromISR(*pxSemaphore, NULL);
}

void Timer1Interrupts_Init(unsigned int uiPeriod, SemaphoreHandle_t *xSemaphore){

	pxSemaphore = xSemaphore;

	VICIntEnable |= (1 << VIC_TIMER1_CHANNEL_NR);
	VICVectCntl1  = mIRQ_SLOT_ENABLE | VIC_TIMER1_CHANNEL_NR;
	VICVectAddr1  =(unsigned long)Timer1IRQHandler;

	T1MR0 = PCLK_CLOCK * uiPeriod;
	T1MCR |= (mINTERRUPT_ON_MR0 | mRESET_ON_MR0); 

	T1TCR |=  mCOUNTER_ENABLE;

}
