#include "FreeRTOS.h"
#include "queue.h"
#include "watch.h"

/****************global variables**************/
xQueueHandle xWatchQueue;

/*******************threads*******************/
void Watch_Thread(void *pvParameters) {
	
	
	struct WatchEvent sWatchSecondsEvent;
	struct WatchEvent sWatchMinutesEvent;
	
	sWatchSecondsEvent.eTimeUnit = SECONDS;
	sWatchSecondsEvent.TimeValue = 0;
	
	sWatchMinutesEvent.eTimeUnit = MINUTES;
	sWatchMinutesEvent.TimeValue = 0;
	
	while(1) {
		
		if(sWatchSecondsEvent.TimeValue < 60) {
			sWatchSecondsEvent.TimeValue++;
		}
		else {
			sWatchSecondsEvent.TimeValue = 0;
			sWatchMinutesEvent.TimeValue++;
			xQueueSendToBack(xWatchQueue, &sWatchMinutesEvent, portMAX_DELAY);
		}
		xQueueSendToBack(xWatchQueue, &sWatchSecondsEvent, portMAX_DELAY);
		vTaskDelay(1000);
	}
}

/*******************functions*******************/
void WatchInit(void) {
	xWatchQueue = xQueueCreate(1, sizeof(struct WatchEvent));
	xTaskCreate(Watch_Thread, NULL, 128, NULL, 1, NULL );
}

struct WatchEvent sWatchRead(void) {
	
	struct WatchEvent sWatchEvent;
	
	xQueueReceive(xWatchQueue, &sWatchEvent, 0);
	return sWatchEvent;
}
