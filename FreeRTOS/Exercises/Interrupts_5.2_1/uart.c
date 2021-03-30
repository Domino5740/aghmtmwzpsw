#include <LPC210X.H>
#include "uart.h"
#include "string.h"

#define NULL 00

/************ UART ************/

#define mP0_1_RX0_PIN_MODE 						   0x00000004
#define mP0_0_TX0_PIN_MODE 						   0x00000001

// U0LCR Line Control Register
#define mDIVISOR_LATCH_ACCESS_BIT                  0x00000080
#define m8BIT_UART_WORD_LENGTH                     0x00000003

// UxIER Interrupt Enable Register
#define mRX_DATA_AVALAIBLE_INTERRUPT_ENABLE        0x00000001
#define mTHRE_INTERRUPT_ENABLE                     0x00000002

// UxIIR Pending Interrupt Identification Register
#define mINTERRUPT_PENDING_IDETIFICATION_BITFIELD  0x0000000F
#define mTHRE_INTERRUPT_PENDING                    0x00000002
#define mRX_DATA_AVALAIBLE_INTERRUPT_PENDING       0x00000004

/************ Interrupts **********/
// VIC (Vector Interrupt Controller) channels
#define VIC_UART0_CHANNEL_NR  6
#define VIC_UART1_CHANNEL_NR  7

// VICVectCntlx Vector Control Registers
#define mIRQ_SLOT_ENABLE                           0x00000020

////////////// Zmienne globalne ///////////
char cOdebranyZnak;
struct UartRxBuffer sUartRxBuffer;
struct UartTxBuffer sUartTxBuffer;

unsigned char ucCounter = 0;

////////////////////////////////////////////
void UartRx_PutCharacterToBuffer(char cCharacter) {
	
	if(sUartRxBuffer.ucCharCtr >= UART_RX_BUFFER_SIZE) {
		sUartRxBuffer.eStatus = OVERFLOW;
	}
	else {
		if(cCharacter == TERMINATOR) {
			sUartRxBuffer.cData[sUartRxBuffer.ucCharCtr] = '\0';
			sUartRxBuffer.eStatus = READY;
			sUartRxBuffer.ucCharCtr = 0;
		}
		else {
			sUartRxBuffer.cData[sUartRxBuffer.ucCharCtr] = cCharacter;
			sUartRxBuffer.ucCharCtr += 1;
		}
	}
}

enum UartRxStatus eUartRx_GetStatus(void){
	return sUartRxBuffer.eStatus;
}	

void Uart_GetStringCopy(char *cDestination){
	
	sUartRxBuffer.ucCharCtr = 0;
	
	while(sUartRxBuffer.cData[sUartRxBuffer.ucCharCtr] != '\0') {
		cDestination[sUartRxBuffer.ucCharCtr] = sUartRxBuffer.cData[sUartRxBuffer.ucCharCtr];
		sUartRxBuffer.ucCharCtr++;
	}
	
	cDestination[sUartRxBuffer.ucCharCtr] = '\0';
	
	sUartRxBuffer.eStatus = EMPTY;
	sUartRxBuffer.ucCharCtr = 0;
}	

////////////////////////////////////////////
char UartTx_GetCharacterFromBuffer(void) {
	
	char cData;
	
	if(sUartTxBuffer.eStatus == BUSY) {
		
		cData = sUartTxBuffer.cData[sUartTxBuffer.ucCharCtr];
		sUartTxBuffer.ucCharCtr++;
		
		if(sUartTxBuffer.fLastCharacter == 1) {
			cData = NULL;
			sUartTxBuffer.fLastCharacter = 0;
			sUartTxBuffer.ucCharCtr = 0;
			sUartTxBuffer.eStatus = FREE;
		}
		else if(cData == NULL) {
			cData = TERMINATOR;
			sUartTxBuffer.fLastCharacter = 1;
		}
	}
	return cData;
}

void Transmitter_SendString(char cString[]) {
	
	CopyString(cString, sUartTxBuffer.cData);
	U0THR = sUartTxBuffer.cData[0];
	sUartTxBuffer.eStatus = BUSY;
	sUartTxBuffer.fLastCharacter = 0;
	sUartTxBuffer.ucCharCtr = 1;
}

enum UartTxStatus eUartTx_GetStatus(void) {
	return sUartTxBuffer.eStatus;
}

///////////////////////////////////////////
__irq void UART0_Interrupt (void) {
   
   unsigned int uiCopyOfU0IIR = U0IIR; // odczyt U0IIR powoduje jego kasowanie wiec lepiej pracowac na kopii
   char cCharacterFromBuffer;

   if ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mRX_DATA_AVALAIBLE_INTERRUPT_PENDING) { // odebrano znak
     cOdebranyZnak = U0RBR;
	 UartRx_PutCharacterToBuffer(cOdebranyZnak);
   }
   
   if ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mTHRE_INTERRUPT_PENDING) { // wyslano znak - nadajnik pusty
		if(eUartTx_GetStatus() == BUSY) {
			cCharacterFromBuffer = UartTx_GetCharacterFromBuffer();
			U0THR = cCharacterFromBuffer;
		}
	}
	
   VICVectAddr = 0; // Acknowledge Interrupt
}

////////////////////////////////////////////
void UART_InitWithInt(unsigned int uiBaudRate){
	
	unsigned long ulDivisor, ulWantedClock;
	ulWantedClock=uiBaudRate*16;
	ulDivisor=15000000/ulWantedClock;
	// UART
	PINSEL0 = PINSEL0 | 0x55;                                     // ustawic piny uar0 odbiornik nadajnik
	U0LCR  |= m8BIT_UART_WORD_LENGTH | mDIVISOR_LATCH_ACCESS_BIT; // d³ugosc s³owa, DLAB = 1
	U0DLL = ( unsigned char ) ( ulDivisor & ( unsigned long ) 0xff );
	ulDivisor >>= 8;
	U0DLM = ( unsigned char ) ( ulDivisor & ( unsigned long ) 0xff );
	U0LCR  &= (~mDIVISOR_LATCH_ACCESS_BIT);                       // DLAB = 0
	U0IER  |= mRX_DATA_AVALAIBLE_INTERRUPT_ENABLE | mTHRE_INTERRUPT_ENABLE ;               

	// INT
	VICVectAddr1  = (unsigned long) UART0_Interrupt;             // set interrupt service routine address
	VICVectCntl1  = mIRQ_SLOT_ENABLE | VIC_UART0_CHANNEL_NR;     // use it for UART 0 Interrupt
	VICIntEnable |= (0x1 << VIC_UART0_CHANNEL_NR);               // Enable UART 0 Interrupt Channel

}
/*stara wersja inicjalizacji
powyzsza jest bardziej skomplikowana dla mozliwosci ustawienia baud rate 300
void UART_InitWithInt(unsigned int uiBaudRate) {

   // UART0
   PINSEL0 = PINSEL0 | mP0_1_RX0_PIN_MODE | mP0_0_TX0_PIN_MODE;                                     // ustawic pina na odbiornik uart0
   U0LCR  |= m8BIT_UART_WORD_LENGTH | mDIVISOR_LATCH_ACCESS_BIT; // dlugosc slowa, DLAB = 1
   U0DLL   = ((15000000/16)/uiBaudRate);                         // predkosc transmisji
   U0LCR  &= (~mDIVISOR_LATCH_ACCESS_BIT);                       // DLAB = 0
   U0IER  |= mRX_DATA_AVALAIBLE_INTERRUPT_ENABLE | mTHRE_INTERRUPT_ENABLE;                // wlaczamy przerwania na linii Rx data
	
   // INT
   VICVectAddr1  = (unsigned long)UART0_Interrupt;           // set interrupt service routine address
   VICVectCntl1  = mIRQ_SLOT_ENABLE | VIC_UART0_CHANNEL_NR;     // use it for UART 0 Interrupt
   VICIntEnable |= (0x1 << VIC_UART0_CHANNEL_NR);           // Enable UART 0 Interrupt Channel
}*/
