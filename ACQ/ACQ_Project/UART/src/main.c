#include <asf.h>

#define UART_TXPIN IOPORT_CREATE_PIN(PORTE, 3)
#define UART_RX_BUFFER_SIZE 20

int main (void) {
	static usart_rs232_options_t USART_SERIAL_OPTIONS = {
		.baudrate = 9600,
		.charlength = USART_CHSIZE_8BIT_gc,
		.paritytype = USART_PMODE_DISABLED_gc,
		.stopbits = false
	};
	stdio_serial_init(&USARTE0, &USART_SERIAL_OPTIONS);
	ioport_set_pin_dir(UART_TXPIN, IOPORT_DIR_OUTPUT);
	
	char ch;
	char str[UART_RX_BUFFER_SIZE];
	int i = 0;
	
	while(1) {
		
		//zad.2:
		/*
		fgets(str, UART_RX_BUFFER_SIZE, stdin);
		fputs(str, stdout);
		*/
		
		//zad.3:
		/*
		scanf("%c",&ch);
		switch(ch) {
			case 'b':
				puts("binary");
				break;
			case 'v':
				puts("voltage");
				break;
			case 't':
				puts("temperature");
				break;
			default: 
				puts("???");
				break;
		}
		*/
		
		//zad.4:
		printf("%d\n\r", i);
		i++;
		
	}
}