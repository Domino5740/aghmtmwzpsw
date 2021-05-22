#include "port.h"
#include "spi.h"
#include "nrf24L01.h"
#include "rf.h"

#define RF_PORT PORTC
#define RF_CHANNEL_PORT PORTB

#define SPI_CS_bm	(1<<1)
#define SPI_SS_bm	(1<<4)
#define RF_CE_bm	(1<<0)
#define RF_CHANNEL_bm  0x0f

SPI_Master_t rf_spi_master;

void set_CS(unsigned char uset_CState){
	if (uset_CState) {
		PORT_SetPins( &RF_PORT,SPI_CS_bm);
	}
	else {
		PORT_ClearPins( &RF_PORT,SPI_CS_bm);
	}
};

void set_CE(unsigned char uset_CState){
	if (uset_CState) {
		PORT_SetPins( &RF_PORT,RF_CE_bm);
	}
	else {
		PORT_ClearPins( &RF_PORT,RF_CE_bm);
	}
};

void spi_init(void) {
	PORT_SetPinsAsOutput( &RF_PORT,SPI_CS_bm |SPI_SS_bm);
	set_CS(1);
	SPI_MasterInit(&rf_spi_master,&SPIC,&PORTC,false,SPI_MODE_0_gc,SPI_INTLVL_OFF_gc,false,SPI_PRESCALER_DIV4_gc);
}

void reg_write(unsigned char ucAddr,unsigned char ucValue){
	set_CS(0);
	SPI_MasterTransceiveByte(&rf_spi_master,WRITE_REG|ucAddr);
	SPI_MasterTransceiveByte(&rf_spi_master,ucValue);
	set_CS(1);
};

unsigned char reg_read(unsigned char ucAddr){
	unsigned char  ucValue;
	set_CS(0);
	delay_us(20);
	SPI_MasterTransceiveByte(&rf_spi_master,ucAddr);
	ucValue = SPI_MasterTransceiveByte(&rf_spi_master,0);
	set_CS(1);
	return ucValue;
};

void payload_write( unsigned char uPayload[],unsigned char ucLength){
	unsigned char ucCtr;
	set_CS(0);
	SPI_MasterTransceiveByte(&rf_spi_master,WR_TX_PLOAD);
	for (ucCtr=0;ucCtr<ucLength;ucCtr++) SPI_MasterTransceiveByte(&rf_spi_master,uPayload[ucCtr]);
	set_CS(1);
};

void address_write(unsigned char ucAddr[], unsigned char ucLength) {
	unsigned char ucCtr;
	set_CS(0);
	SPI_MasterTransceiveByte(&rf_spi_master,TX_ADDR);
	for (ucCtr=0;ucCtr<ucLength;ucCtr++) SPI_MasterTransceiveByte(&rf_spi_master,ucAddr[ucCtr]);
	set_CS(1);
}

// In status reg: read bit value and clear bit if it was set
unsigned char status_bit_test(unsigned char ucBitPosition){
	
	unsigned char ucStatus, ucBitMask = 1 << ucBitPosition;

	set_CS(0);
	ucStatus=SPI_MasterTransceiveByte(&rf_spi_master,0xff);
	set_CS(1);
	
	if ((ucStatus & ucBitMask) != 0) {
		reg_write(STATUS_, ucBitMask); // clear bit
		return 1;
		} else{
		return 0;
	}
}

void set_bit_in_reg(unsigned char ucAddr,unsigned char ucBitPosition, unsigned char ucBitValue){
	unsigned char ucBitMask=1<<ucBitPosition;
	unsigned char ucCurrentRegValue=reg_read(ucAddr);
	
	if(ucBitValue==1){
		reg_write(ucAddr,ucCurrentRegValue | ucBitMask);
		}else{
		reg_write(ucAddr,ucCurrentRegValue & (~ucBitMask));
	}
}

void rf_init_as_tx(void){
	delay_us(11000);
	
	spi_init();
	PORT_SetPinsAsOutput( &RF_PORT,RF_CE_bm);
	
	set_CE(0);

	reg_write(CONFIG,(1<<PWR_UP)|(1<<EN_CRC)|(1<<MASK_TX_DS));
	reg_write(EN_AA,0);
	reg_write(RF_SETUP,(1<<RF_DR));
	reg_write(RF_CH, 19);
	
	delay_us(2000);
}

void rf_clear_int_flags(){
	reg_write(STATUS_,0xff);
}

void rf_send_byte(unsigned char ucByte) {
	unsigned char ucRxAddress = 19;
	
	set_CE(1);
	//address_write(&ucRxAddress, 1);
	payload_write(&ucByte, 1);
	set_CE(0);
}



