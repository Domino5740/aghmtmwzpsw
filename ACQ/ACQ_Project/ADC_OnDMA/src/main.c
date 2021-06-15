#include <asf.h>

#define UART_TXPIN IOPORT_CREATE_PIN(PORTE, 3)
#define MY_LED    IOPORT_CREATE_PIN(PORTD, 5)
#define ULTRA_TRIGGER IOPORT_CREATE_PIN(PORTA, 7)
#define MY_ADC    ADCA
#define MY_ADC_CH ADC_CH0

#define ADC_GAIN 1
#define ADC_CHANNEL_NR 1
#define ADC_DEFAULT_CH 0

#define DMA_CHANNEL 0
#define SAMPLES_NR 10000

uint8_t Samples[SAMPLES_NR];

static void adc_init(void) {
	struct adc_config adc_conf;
	struct adc_channel_config adcch_conf;
	
	adc_read_configuration(&MY_ADC, &adc_conf);
	adcch_read_configuration(&MY_ADC, MY_ADC_CH, &adcch_conf);
	
	adc_set_conversion_parameters(&adc_conf, ADC_SIGN_OFF, ADC_RES_8, ADC_REF_AREFA);
	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_FREERUN, ADC_CHANNEL_NR, ADC_DEFAULT_CH);
	adc_set_clock_rate(&adc_conf, 1000000UL);

	adcch_enable_interrupt(&adcch_conf);
	adcch_set_input(&adcch_conf, ADCCH_POS_PIN1, ADCCH_NEG_NONE, ADC_GAIN);
	
	adc_write_configuration(&MY_ADC, &adc_conf);
	adcch_write_configuration(&MY_ADC, MY_ADC_CH, &adcch_conf);
}

static void dma_init(void)
{
	struct dma_channel_config dmach_conf;
	memset(&dmach_conf, 0, sizeof(dmach_conf));
	
	dma_channel_set_burst_length(&dmach_conf, DMA_CH_BURSTLEN_1BYTE_gc);
	dma_channel_set_transfer_count(&dmach_conf, SAMPLES_NR);
	
	dma_channel_set_src_reload_mode(&dmach_conf, DMA_CH_SRCRELOAD_BURST_gc);
	dma_channel_set_dest_reload_mode(&dmach_conf, DMA_CH_DESTRELOAD_TRANSACTION_gc);
	dma_channel_set_src_dir_mode(&dmach_conf, DMA_CH_SRCDIR_INC_gc);
	dma_channel_set_dest_dir_mode(&dmach_conf, DMA_CH_DESTDIR_INC_gc);
	dma_channel_set_source_address(&dmach_conf, (uint16_t)(uintptr_t)&ADCA.CH0RES);
	dma_channel_set_destination_address(&dmach_conf, (uint16_t)(uintptr_t)Samples);
	
	dma_channel_set_trigger_source(&dmach_conf, DMA_CH_TRIGSRC_ADCA_CH0_gc);
	dma_channel_set_single_shot(&dmach_conf);
	
	dma_enable();
	dma_channel_set_interrupt_level(&dmach_conf, DMA_INT_LVL_LO);
	dma_channel_write_config(DMA_CHANNEL, &dmach_conf);
	dma_channel_enable(DMA_CHANNEL);
}

int main (void) {
	// CLK
	sysclk_init();
	// IOPORT
	ioport_init();
	ioport_set_pin_dir(MY_LED, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(ULTRA_TRIGGER, IOPORT_DIR_OUTPUT);
	// UART
	ioport_set_pin_dir(UART_TXPIN, IOPORT_DIR_OUTPUT);
	static usart_rs232_options_t USART_SERIAL_OPTIONS = {
		.baudrate = 921600,
		.charlength = USART_CHSIZE_8BIT_gc,
		.paritytype = USART_PMODE_DISABLED_gc,
		.stopbits = false
	};
	usart_serial_init(&USARTE0, &USART_SERIAL_OPTIONS);
	// ADC
	adc_init();
	adc_enable(&MY_ADC);
	// DMA
	dma_init();
	// Main loop
	while(1) {
		uint8_t c;
		usart_serial_getchar(&USARTE0,&c);
		
		ioport_set_pin_level(ULTRA_TRIGGER, true);
		delay_ms(1);
		ioport_set_pin_level(ULTRA_TRIGGER, false);
		ioport_toggle_pin_level(MY_LED);
		
		dma_channel_enable(DMA_CHANNEL);
		while(dma_channel_is_busy(DMA_CHANNEL));
		usart_serial_write_packet(&USARTE0, Samples, SAMPLES_NR);
	};
}