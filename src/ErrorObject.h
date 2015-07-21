#ifndef ErrorObject_H
#define ErrorObject_H

typedef enum{
    ERR_NOT_OPERATOR,
    ERR_UNDEFINED_OPERATOR,
    ERR_ILLEGAL_CHARACTER,
    ERR_NOT_NUMBER,
    ERR_ILLEGAL_PREFIX,
    ERR_NULL_TOKEN
} ErrorCode;


typedef struct{
  char *errorMsg;
  ErrorCode errorCode;
} ErrorObject;

void freeError(ErrorObject *errObj);
void ThrowError(char *message, ErrorCode errCode);

#endif // ErrorObject_H
