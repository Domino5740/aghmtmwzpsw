#define UART_RX_BUFFER_SIZE 50
#define TERMINATOR '\r'
#define UART_TX_BUFFER_SIZE 50

enum UartRxStatus {RECEIVING, DONE, OVERFLOW};

typedef struct UartRxBuffer {
	enum UartRxStatus eStatus;
	char cData[UART_RX_BUFFER_SIZE];
	unsigned char ucCharCtr;
} UartRxBuffer;

__irq void UART0_Interrupt (void);
void UART_InitWithInt(unsigned int uiBaudRate);

void Uart_GetString(char *pReceivedString);

void Uart_PutString(char *pStringToBeSend);
