#include <malloc.h>
#include "ErrorObject.h"
#include "CException.h"

void ThrowError(char *message, ErrorCode errCode){
  ErrorObject *errObj = malloc(sizeof(ErrorObject));
  errObj->errorMsg = message;
  errObj->errorCode = errCode;
  Throw(errObj);
}

void freeError(ErrorObject *errObj){
  free(errObj);
}
