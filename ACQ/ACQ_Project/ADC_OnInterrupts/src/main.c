#include <asf.h>

#define UART_TXPIN IOPORT_CREATE_PIN(PORTE, 3)
#define MY_ADC    ADCA
#define MY_ADC_CH ADC_CH0
#define MY_TIMER  TCC0
#define TIMER_TRIGGER_FREQ 50000


static void evsys_init(void) {
	sysclk_enable_module(SYSCLK_PORT_GEN, SYSCLK_EVSYS);
	EVSYS.CH0MUX = EVSYS_CHMUX_TCC0_OVF_gc;
}

static void tc_init(void) {
	tc_enable(&MY_TIMER);
	tc_set_wgm(&MY_TIMER, TC_WG_NORMAL);
	tc_set_resolution(&MY_TIMER, 500000);
	tc_write_period(&MY_TIMER, tc_get_resolution(&MY_TIMER) / TIMER_TRIGGER_FREQ);
}

static void adc_handler(ADC_t *adc, uint8_t ch_mask, adc_result_t result)
{
	switch (ch_mask) {
		case ADC_CH0:
			usart_serial_putchar(&USARTE0, (uint8_t) (result & 0xFF));
		break;
		default:
		break;
	}
}


static void adc_init(void) {
	struct adc_config adc_conf;
	struct adc_channel_config adcch_conf;
	
	adc_read_configuration(&MY_ADC, &adc_conf);
	adcch_read_configuration(&MY_ADC, MY_ADC_CH, &adcch_conf);
	
	adc_set_conversion_parameters(&adc_conf, ADC_SIGN_OFF, ADC_RES_8, ADC_REF_AREFA);
	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_EVENT_SINGLE, 1, 0);
	adc_set_clock_rate(&adc_conf, 500000UL);
	adc_set_callback(&MY_ADC, &adc_handler);
	
	adcch_enable_interrupt(&adcch_conf);
	adcch_set_input(&adcch_conf, ADCCH_POS_PIN1, ADCCH_NEG_NONE, 1);
	
	adc_write_configuration(&MY_ADC, &adc_conf);
	adcch_write_configuration(&MY_ADC, MY_ADC_CH, &adcch_conf);
}

int main (void) {
	
	static usart_rs232_options_t USART_SERIAL_OPTIONS = {
		.baudrate = 921600,
		.charlength = USART_CHSIZE_8BIT_gc,
		.paritytype = USART_PMODE_DISABLED_gc,
		.stopbits = false
	};
	
	cpu_irq_enable();
	sysclk_init();
	evsys_init();
	tc_init();
	pmic_init();
	usart_serial_init(&USARTE0, &USART_SERIAL_OPTIONS);
	ioport_set_pin_dir(UART_TXPIN, IOPORT_DIR_OUTPUT);
	adc_init();
	adc_enable(&MY_ADC);
	
	while(1) {}
}
