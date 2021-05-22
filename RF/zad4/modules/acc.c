#include "twi.h"
#include "acc.h"

/*! CPU speed 2MHz, BAUDRATE 100kHz and Baudrate Register Settings */
#define CPU_SPEED   2000000
#define BAUDRATE	200000
#define TWI_BAUDSETTING TWI_BAUD(CPU_SPEED, BAUDRATE)

#define SLAVE_ADDRESS 0x32>>1

TWI_Master_t twiMaster; 

uint8_t Buffer[1];

u8_t LIS3DH_ReadReg(u8_t Addr, u8_t* Data) {
	
	Buffer[0]=Addr;
	TWI_MasterWriteRead(&twiMaster,SLAVE_ADDRESS,Buffer,1,1);
	while (twiMaster.status != TWIM_STATUS_READY) {	}
	Data[0]=twiMaster.readData[0];
	
	return 1;
}

u8_t LIS3DH_WriteReg(u8_t Addr, u8_t Data) {
	
	uint8_t Buffer[2];
		
	Buffer[0]=Addr;
	Buffer[1]=Data;
	TWI_MasterWriteRead(&twiMaster,SLAVE_ADDRESS,&Buffer[0],2,0);
	while (twiMaster.status != TWIM_STATUS_READY) {	}
	
	return 1;
}

void acc_init(void){

	TWI_MasterInit(&twiMaster,&TWIE,TWI_MASTER_INTLVL_HI_gc,TWI_BAUDSETTING); //TWI_MASTER_INTLVL_LO_gc
	PMIC.CTRL |= PMIC_HILVLEN_bm;//PMIC_LOLVLEN_bm
	
	sei();
	
	// ACC
	LIS3DH_SetODR(LIS3DH_ODR_400Hz);
	LIS3DH_SetMode(LIS3DH_NORMAL);
	LIS3DH_SetFullScale(LIS3DH_FULLSCALE_2);
	LIS3DH_SetAxis(LIS3DH_X_ENABLE | LIS3DH_Y_ENABLE | LIS3DH_Z_ENABLE);
};

unsigned char acc_read(void){
	unsigned char acc;
	LIS3DH_ReadReg(LIS3DH_OUT_Z_H, &acc);	
	return acc+128; 
}

/*! TWIE Master Interrupt vector. */
ISR(TWIE_TWIM_vect)
{
	TWI_MasterInterruptHandler(&twiMaster);
}

