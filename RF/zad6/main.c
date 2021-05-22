#include "modules/led.h"
#include "modules/rf.h"
#include "modules/avr_compiler.h"
#include "modules/nrf24L01.h"
#include <avr/sleep.h>

#define PORTC3__bm 1 << 3
#define PORTC2__bm 1 << 2

unsigned char cByte = '\n';

ISR(PORTC_INT0_vect) { // interrupt service routine
	rf_send_byte(cByte);
	set_bit_in_reg(STATUS_, TX_DS, 1);
};

int main (void) {
	
	led_init();
	rf_init_as_tx();
	
	PORTC.DIRCLR = PORTC2__bm;
	PORTC.PIN3CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	PORTC.INT0MASK |= PORTC2__bm;
	PORTC.INTCTRL = PORT_INT0LVL_LO_gc;
	PMIC.CTRL |= PMIC_LOLVLEN_bm;
	set_bit_in_reg(STATUS_, TX_DS, 1);
	rf_send_byte(cByte);
	
	sei(); // global interrupts enabling
	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_enable();
	while(1) {
		sleep_cpu();
	};
}