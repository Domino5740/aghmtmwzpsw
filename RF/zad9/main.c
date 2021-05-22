#include "modules/acc.h"
#include "modules/rf.h"
#include "modules/avr_compiler.h"
#include "modules/nrf24L01.h"
#include <avr/sleep.h>

ISR(RTC_OVF_vect) { rf_send_byte(acc_read()); }
int main (void) {
	rf_init_as_tx();
	acc_init();
	RTC.PER  = 10;
	RTC.CNT = 0;
	RTC.CTRL = RTC_PRESCALER_DIV1_gc;
	RTC.INTCTRL = RTC_OVFINTLVL0_bm;
	CLK.RTCCTRL = CLK_RTCEN_bm | CLK_RTCSRC_ULP_gc;
	PMIC.CTRL |= PMIC_LOLVLEN_bm;
	sei();
	set_sleep_mode(SLEEP_MODE_PWR_SAVE);
	sleep_enable();
	while(1){
		sleep_cpu();
	}
}