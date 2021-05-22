#include "modules/led.h"
#include "modules/rf.h"
#include "modules/avr_compiler.h"


int main (void) {
	led_init();
	rf_init_as_tx();
	
	while(1) {
		
		static unsigned char ucMsgCtr = 0;
		
		rf_send_byte(ucMsgCtr);
		rf_wait_until_data_sent();
		led_toggle();
		ucMsgCtr -= 7;
	}
}
