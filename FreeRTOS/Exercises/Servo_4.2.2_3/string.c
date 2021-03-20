#include "string.h"

void CopyString(char pcTokens[], char pcDestination[]) {

	unsigned char ucCharCounter;

	for(ucCharCounter = 0; pcTokens[ucCharCounter] != '\0'; ucCharCounter++) {
		pcDestination[ucCharCounter] = pcTokens[ucCharCounter];
	}
	while (pcDestination[ucCharCounter] != '\0') {
		pcDestination[ucCharCounter] = '\0';
		ucCharCounter++;
	}
}

void AppendString(char pcTokensStr[], char pcDestinationStr[]) {

	unsigned char ucEndPointer;

  for(ucEndPointer = 0; pcDestinationStr[ucEndPointer] != '\0' ; ucEndPointer++) {}
  CopyString(pcTokensStr,pcDestinationStr+ucEndPointer);
}

enum CompResult eCompareString(char pcStr1[], char pcStr2[]) {

    unsigned char ucCharCounter;

    for(ucCharCounter = 0; pcStr1[ucCharCounter] != '\0'; ucCharCounter++) {
        if (pcStr1[ucCharCounter] != pcStr2[ucCharCounter]) {
            return DIFFERENT;
        }
    }
    return EQUAL;
}

void ReplaceCharactersInString(char pcString[], char cOldChar, char cNewChar) {

    unsigned char ucCharCounter;

    for(ucCharCounter = 0; pcString[ucCharCounter] != '\0'; ucCharCounter++) {
        if(pcString[ucCharCounter] == cOldChar) {
            pcString[ucCharCounter] = cNewChar;
        }
    }
}

enum Result eHexStringToUInt(char pcStr[], unsigned int *puiValue) {

    unsigned char ucCharCounter;
    *puiValue = 0;

    if((pcStr[0] != '0') | (pcStr[1] != 'x') | (pcStr[2] == '\0')) {
        return ERR;
    }

    for(ucCharCounter = 2; pcStr[ucCharCounter] != '\0'; ucCharCounter++) {
        if(ucCharCounter > 6) {
            return ERR;
        }
        *puiValue = *puiValue << 4;
        if((pcStr[ucCharCounter] >= 'A') & (pcStr[ucCharCounter] <= 'F')) {
            *puiValue = *puiValue | (pcStr[ucCharCounter] - 'A' + 10);
        }
        else if(pcStr[ucCharCounter] > 'F') {
            return ERR;
        }
        else {
            *puiValue = *puiValue | (pcStr[ucCharCounter] - '0');
        }
    }
    return OK;
}

void UIntToHexStr(unsigned int uiValue, char pcStr[]) {

	unsigned char ucNibbleCounter;
	unsigned char ucCurrentNibble;

	pcStr[0] = '0';
	pcStr[1] = 'x';

	for(ucNibbleCounter = 0; ucNibbleCounter < 4; ucNibbleCounter++) {
		ucCurrentNibble = ((uiValue >> (ucNibbleCounter * 4)) & HEX_bm);
		if(ucCurrentNibble > 9) {
			pcStr[5 - ucNibbleCounter] = (ucCurrentNibble - 10 + 'A');
		}
		else {
			pcStr[5 - ucNibbleCounter] = (ucCurrentNibble + '0');
		}
		pcStr[6] = '\0';
	}
}
