#include "port.h"
#include "led.h"

#define LED_PORT PORTD
#define LED_bm	(1<<4)

void led_init(void){
	PORT_SetPinsAsOutput( &LED_PORT,LED_bm);
}

void led_toggle(void){
	PORT_TogglePins(&LED_PORT,LED_bm);
}

void led_set(void){
	PORT_SetPins( &LED_PORT,LED_bm);
}

void led_clear(void){
	PORT_ClearPins(&LED_PORT,LED_bm);
}
