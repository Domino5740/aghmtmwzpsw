#define HEX_bm 0x000F

enum CompResult {DIFFERENT, EQUAL};
enum Result {OK, ERR};

void CopyString(char pcTokens[], char pcDestination[]);
void AppendString(char pcTokensStr[], char pcDestinationStr[]);
enum CompResult eCompareString(char pcStr1[], char pcStr2[]);
void ReplaceCharactersInString(char pcString[], char cOldChar, char cNewChar);
enum Result eHexStringToUInt(char pcStr[],unsigned int *puiValue);
void UIntToHexStr(unsigned int uiValue, char pcStr[]);
