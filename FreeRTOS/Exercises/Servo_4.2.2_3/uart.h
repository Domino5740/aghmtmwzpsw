#define RECEIVER_SIZE 30
#define TERMINATOR '\r'
#define TRANSMITTER_SIZE 30

enum eReceiverStatus {EMPTY, READY, OVERFLOW};
typedef struct ReceiverBuffer {
	char cData[RECEIVER_SIZE];
	unsigned char ucCharCtr;
	enum eReceiverStatus eStatus;
} ReceiverBuffer;

enum eTransmitterStatus {FREE, BUSY};
typedef struct TransmitterBuffer {
	char cData [TRANSMITTER_SIZE];
	enum eTransmitterStatus eStatus;
	unsigned char fLastCharacter;
	unsigned char ucCharCtr;
} TransmitterBuffer;

__irq void UART0_Interrupt (void);
void UART_InitWithInt(unsigned int uiBaudRate);

void Receiver_PutCharacterToBuffer(char cCharacter);
enum eReceiverStatus Receiver_GetStatus(void);
void Receiver_GetStringCopy(char *cDestination);

char Transmitter_GetCharacterFromBuffer(void);
void Transmitter_SendString(char cString[]);
enum eTransmitterStatus Transmitter_GetStatus(void);
