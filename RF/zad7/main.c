#include "modules/led.h"
#include "modules/rf.h"
#include "modules/avr_compiler.h"
#include "modules/nrf24L01.h"
#include <avr/sleep.h>

#define PORTC3__bm 1 << 3
#define PORTC2__bm 1 << 2

ISR(RTC_OVF_vect) { led_set(); }
ISR(RTC_COMP_vect) {led_clear();}
int main (void) {
	led_init();
	RTC.PER  = 1024;
	RTC.CNT = 0;
	RTC.COMP = 50; //1024Hz / 50 ~= 20Hz, = 50ms period
	RTC.CTRL = RTC_PRESCALER_DIV1_gc;
	RTC.INTCTRL = RTC_COMPINTLVL0_bm | RTC_OVFINTLVL0_bm;
	CLK.RTCCTRL = CLK_RTCEN_bm | CLK_RTCSRC_ULP_gc;
	PMIC.CTRL |= PMIC_LOLVLEN_bm;
	sei();
	//set_sleep_mode(SLEEP_MODE_PWR_DOWN); w tym trybie nie dzia³a RTC, chyba o power save chodiz³o
	set_sleep_mode(SLEEP_MODE_PWR_SAVE);
	sleep_enable();
	while(1){
		sleep_cpu();
	}
}