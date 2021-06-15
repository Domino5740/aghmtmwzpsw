#include <asf.h>

#define UART_TXPIN IOPORT_CREATE_PIN(PORTE, 3)
#define MY_ADC    ADCA
#define MY_ADC_CH ADC_CH0
#define MY_TIMER  TCC0

static void evsys_init(void)
{
	sysclk_enable_module(SYSCLK_PORT_GEN, SYSCLK_EVSYS);
	
	EVSYS.CH0MUX = EVSYS_CHMUX_OFF_gc;
}

static void adc_init(void)
{
	struct adc_config adc_conf;
	struct adc_channel_config adcch_conf;
	
	adc_read_configuration(&MY_ADC, &adc_conf);
	adcch_read_configuration(&MY_ADC, MY_ADC_CH, &adcch_conf);
	
	adc_set_conversion_parameters(&adc_conf, ADC_SIGN_OFF, ADC_RES_8, ADC_REF_AREFA);
	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_EVENT_SINGLE, 1, 0);
	adc_set_clock_rate(&adc_conf, 200000UL);
	
	adcch_set_input(&adcch_conf, ADCCH_POS_PIN1, ADCCH_NEG_NONE, 1);
	
	adc_write_configuration(&MY_ADC, &adc_conf);
	adcch_write_configuration(&MY_ADC, MY_ADC_CH, &adcch_conf);
}

int main (void) {
	
	uint16_t result = 0;
	char received_char;
	static usart_rs232_options_t USART_SERIAL_OPTIONS = {
		.baudrate = 9600,
		.charlength = USART_CHSIZE_8BIT_gc,
		.paritytype = USART_PMODE_DISABLED_gc,
		.stopbits = false
	};
	
	sysclk_init();
	evsys_init();
	stdio_serial_init(&USARTE0, &USART_SERIAL_OPTIONS);
	ioport_set_pin_dir(UART_TXPIN, IOPORT_DIR_OUTPUT);
	adc_init();
	adc_enable(&MY_ADC);
	
	while(1) {
		scanf("%c", &received_char);
		EVSYS.STROBE = 0x01; //trigger event
		if(adc_get_interrupt_flag(&MY_ADC, ADC_CH0) == ADC_CH0) {
			result = adc_get_result(&MY_ADC, MY_ADC_CH);
		}
		printf("%u\r",result);
	}
}
