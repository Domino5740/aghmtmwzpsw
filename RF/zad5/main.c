#include "modules/led.h"
#include "modules/avr_compiler.h"
#include <avr/sleep.h>

#define PORTC3__bm 1 << 3

ISR(PORTC_INT0_vect) { // interrupt service routine
	led_toggle();
};
int main (void) {
	led_init();
	PORTC.DIRCLR = PORTC3__bm;
	PORTC.PIN3CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	PORTC.INT0MASK |= PORTC3__bm;
	PORTC.INTCTRL = PORT_INT0LVL_LO_gc;
	PMIC.CTRL |= PMIC_LOLVLEN_bm;
	sei(); // global interrupts enabling
	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_enable();
	while(1) {
		sleep_cpu();
	};
}