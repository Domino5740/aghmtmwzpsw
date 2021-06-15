#include <asf.h>
#define UART_TXPIN IOPORT_CREATE_PIN(PORTE, 3)
#define SAMPLES_NR 10000
uint8_t Samples[SAMPLES_NR];

int main(void){
	// CLK
	sysclk_init();
	// UART
	ioport_set_pin_dir(UART_TXPIN, IOPORT_DIR_OUTPUT);
	static usart_rs232_options_t USART_SERIAL_OPTIONS = {
		.baudrate = 921600,
		.charlength = USART_CHSIZE_8BIT_gc,
		.paritytype = USART_PMODE_DISABLED_gc,
		.stopbits = false
	};
	usart_serial_init(&USARTE0, &USART_SERIAL_OPTIONS);
	// DATA
	for (int16_t i = 0; i < SAMPLES_NR; i++) {
		Samples[i] = i;
	}
	while(1) {
		uint8_t c;
		usart_serial_getchar(&USARTE0, &c);
		for(int16_t i = 0; i < SAMPLES_NR; i++) {
			Samples[i] = Samples[i] + 1;
		}
		usart_serial_write_packet(&USARTE0, Samples, SAMPLES_NR);
	}
}