#include "command_decoder.h"
#include "string.h"

unsigned char ucTokenNr;

struct Token asToken[MAX_TOKEN_NR];
struct Keyword asKeywordList[MAX_KEYWORD_NR] = {
    {ID, "id"},
    {CALIB, "callib"},
	{GOTO, "goto"},
	{BUTTON, "button"},
	{STATE, "state"}
};

unsigned char ucFindTokensInString(char *pcString) {
	
	char cCurrentChar;
    enum TokenState {TOKEN, DELIMITER};
    enum TokenState eTokenState = DELIMITER;

    unsigned char ucTokenCounter = 0;
	ucTokenNr = 0;

    for(ucTokenCounter = 0; ; ucTokenCounter++) {

        cCurrentChar = pcString[ucTokenCounter];

        switch(eTokenState) {
			case TOKEN:
                if (cCurrentChar == ' ') {
                    eTokenState = DELIMITER;
                }
                else if (cCurrentChar == '\0') {
                    return ucTokenNr;
                }
                else if(ucTokenNr == MAX_TOKEN_NR) {
                    return ucTokenNr;
                }
                break;
            case DELIMITER:
                if(cCurrentChar == '\0') {
                    return ucTokenNr;
                }
                else if(cCurrentChar != ' ') {
                    eTokenState = TOKEN;
                    asToken[ucTokenNr].uValue.pcString = pcString + ucTokenCounter;
                    ucTokenNr++;
                }
                break;
        }
    }
}

enum Result eStringToKeyword (char pcStr[], enum KeywordCode *peKeywordCode) {

    unsigned char ucKeywordCounter;

    for(ucKeywordCounter = 0; ucKeywordCounter < MAX_KEYWORD_NR; ucKeywordCounter++) {
        if(eCompareString(pcStr, asKeywordList[ucKeywordCounter].cString) == EQUAL) {
            *peKeywordCode = asKeywordList[ucKeywordCounter].eCode;
            return OK;
        }
    }
    return ERR;
}

void DecodeTokens(void) {

    unsigned char ucTokenCounter;

    for(ucTokenCounter = 0; ucTokenCounter < ucTokenNr; ucTokenCounter++) {
        if(eStringToKeyword(asToken[ucTokenCounter].uValue.pcString, &asToken[ucTokenCounter].uValue.eKeyword) == OK) {
            asToken[ucTokenCounter].eType = KEYWORD;
        }
        else if(eHexStringToUInt(asToken[ucTokenCounter].uValue.pcString, &asToken[ucTokenCounter].uValue.uiNumber) == OK) {
            asToken[ucTokenCounter].eType = NUMBER;
        }
        else {
            asToken[ucTokenCounter].eType = STRING;
            asToken[ucTokenCounter].uValue.pcString = asToken[ucTokenCounter].uValue.pcString;
        }
    }
}

void DecodeMsg(char *pcString) {
    ucTokenNr = ucFindTokensInString(pcString);
    ReplaceCharactersInString(pcString, ' ', '\0');
    DecodeTokens();
}
