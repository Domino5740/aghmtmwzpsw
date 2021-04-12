#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

__irq void Timer1IRQHandler(void);
void Timer1Interrupts_Init(unsigned int uiPeriod, SemaphoreHandle_t *xSemaphore);
