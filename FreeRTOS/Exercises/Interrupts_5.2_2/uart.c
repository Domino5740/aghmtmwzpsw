#include <LPC21XX.H>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "uart.h"
#include "string.h"

/************ DEFINE's ************/
#define UART_RX_BUFFER_SIZE 30

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
//struct UartRxBuffer sUartRxBuffer;
struct UartTxBuffer sUartTxBuffer;

QueueHandle_t xRxDataQueue;

unsigned char ucCounter = 0;

////////////////////////////////////////////

char cUart_GetChar(void) {
	char cReceivedChar;
	xQueueReceive(xRxDataQueue, &cReceivedChar, portMAX_DELAY);
	return cReceivedChar;
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
	 xQueueSendToBackFromISR(xRxDataQueue, &cOdebranyZnak, 0);
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
	//QUEUE
	xRxDataQueue = xQueueCreate(UART_RX_BUFFER_SIZE, sizeof(char));
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
