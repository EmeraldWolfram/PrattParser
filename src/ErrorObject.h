#ifndef ErrorObject_H
#define ErrorObject_H

typedef enum{
    ERR_UNKNOWN_TOKEN,
	ERR_NOT_OPERATOR,								//
    ERR_UNDEFINED_OPERATOR,							
    ERR_ILLEGAL_CHARACTER,							
    ERR_NOT_NUMBER,									
    ERR_UNEXPECTED_INTEGER,							
    ERR_UNEXPECTED_IDENTIFIER,						
    ERR_ILLEGAL_PREFIX								
} ErrorCode;


typedef struct{
  char *errorMsg;									//errorMsg give a message about the error.
  ErrorCode errorCode;								//ErrorCode is to show the type of error that throw in the function.
} ErrorObject;

void freeError(ErrorObject *errObj);
void ThrowError(char *message, ErrorCode errCode);

#endif // ErrorObject_H

