#define UART_RX_BUFFER_SIZE 30
#define TERMINATOR '\r'
#define TRANSMITTER_SIZE 30

enum UartRxStatus {RECEIVING, DONE, OVERFLOW};

typedef struct UartRxBuffer {
	char cData[UART_RX_BUFFER_SIZE];
	unsigned char ucCharCtr;
	enum UartRxStatus eStatus;
} UartRxBuffer;

enum UartTxStatus {FREE, BUSY};
typedef struct UartTxBuffer {
	char cData [TRANSMITTER_SIZE];
	enum UartTxStatus eStatus;
	unsigned char fLastCharacter;
	unsigned char ucCharCtr;
} UartTxBuffer;

__irq void UART0_Interrupt (void);
void UART_InitWithInt(unsigned int uiBaudRate);

void Uart_GetString(char *pReceivedString);

char Uart_GetCharacterFromBuffer(void);
enum UartTxStatus eUartTx_GetStatus(void);
void Uart_SendString(char cString[]);
