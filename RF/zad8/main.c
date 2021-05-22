#include "modules/led.h"
#include "modules/rf.h"
#include "modules/avr_compiler.h"
#include "modules/nrf24L01.h"
#include <avr/sleep.h>

#define PORTC3__bm 1 << 3
#define PORTC2__bm 1 << 2

unsigned char ucByte = 0;

ISR(RTC_OVF_vect) { rf_send_byte(ucByte++); }
int main (void) {
	rf_init_as_tx();
	RTC.PER  = 10;
	RTC.CNT = 0;
	RTC.CTRL = RTC_PRESCALER_DIV1_gc;
	RTC.INTCTRL = RTC_OVFINTLVL0_bm;
	CLK.RTCCTRL = CLK_RTCEN_bm | CLK_RTCSRC_ULP_gc;
	PMIC.CTRL |= PMIC_LOLVLEN_bm;
	sei();
	set_sleep_mode(SLEEP_MODE_PWR_SAVE);
	sleep_enable();
	rf_send_byte(ucByte++);
	while(1){
		sleep_cpu();
	}
}