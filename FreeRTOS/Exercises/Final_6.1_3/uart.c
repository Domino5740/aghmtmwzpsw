#include <LPC21XX.H>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "uart.h"
#include "string.h"

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
QueueHandle_t xRxDataQueue;
QueueHandle_t xTxDataQueue;

////////////// UartRxFunctions /////////////

void Uart_GetString(char *pReceivedString) {
	
	struct UartRxBuffer sUartRxBuffer;
	
	sUartRxBuffer.ucCharCtr = 0;
	sUartRxBuffer.eStatus = RECEIVING;
	
	while(sUartRxBuffer.eStatus == RECEIVING) {
		if(sUartRxBuffer.ucCharCtr >= UART_RX_BUFFER_SIZE) {
			sUartRxBuffer.eStatus = OVERFLOW;
		}
		else {
			xQueueReceive(xRxDataQueue, &sUartRxBuffer.cData[sUartRxBuffer.ucCharCtr], portMAX_DELAY);
			if(sUartRxBuffer.cData[sUartRxBuffer.ucCharCtr] == TERMINATOR) {
				sUartRxBuffer.cData[sUartRxBuffer.ucCharCtr] = '\0';
				sUartRxBuffer.eStatus = DONE;
			}
			else {
				sUartRxBuffer.ucCharCtr += 1;
			}
		}
	}
	
	if(sUartRxBuffer.eStatus == DONE) {
		CopyString(sUartRxBuffer.cData, pReceivedString);
	}
	else if(sUartRxBuffer.eStatus == OVERFLOW) {
		CopyString("\nOVERFLOW ERROR!\n", pReceivedString);
		xQueueReset(xRxDataQueue);
	}
}

////////////// UartTxFunctions /////////////
void Uart_PutString(char *pCharToBeSend) {
	
	unsigned char ucCharCtr = 0;
	char cTerminator = TERMINATOR;
	char cCharToBeSend;
	
	while((pCharToBeSend[ucCharCtr] != NULL) && (ucCharCtr < UART_TX_BUFFER_SIZE - 1)) {
		xQueueSendToBack(xTxDataQueue, &pCharToBeSend[ucCharCtr], portMAX_DELAY);
		ucCharCtr++;
	}
	xQueueSendToBack(xTxDataQueue, &cTerminator, portMAX_DELAY);
	xQueueReceive(xTxDataQueue, &cCharToBeSend, portMAX_DELAY);
	U0THR = cCharToBeSend;
}

///////////////////////////////////////////
__irq void UART0_Interrupt (void) {

   unsigned int uiCopyOfU0IIR = U0IIR; // odczyt U0IIR powoduje jego kasowanie wiec lepiej pracowac na kopii
   char cCharToBeSend;
   char cReceivedChar;
	

   if ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mRX_DATA_AVALAIBLE_INTERRUPT_PENDING) { // odebrano znak
     cReceivedChar = U0RBR;
	 xQueueSendToBackFromISR(xRxDataQueue, &cReceivedChar, NULL);
   }
   
   if ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mTHRE_INTERRUPT_PENDING) {	   //wyslano znak

      if(xQueueReceiveFromISR(xTxDataQueue, &cCharToBeSend, NULL) == pdTRUE) {
	     if(cCharToBeSend != NULL) {
			U0THR = cCharToBeSend;
		 }
	  }
   }
	
   VICVectAddr = 0; // Acknowledge Interrupt
}

////////////////////////////////////////////
void UART_InitWithInt(unsigned int uiBaudRate){
	
	unsigned long ulDivisor, ulWantedClock;
	ulWantedClock=uiBaudRate*16;
	ulDivisor=15000000/ulWantedClock;
	
	//QUEUES
	xRxDataQueue = xQueueCreate(UART_RX_BUFFER_SIZE, sizeof(char));
	xTxDataQueue = xQueueCreate(UART_TX_BUFFER_SIZE, sizeof(char));
	
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
