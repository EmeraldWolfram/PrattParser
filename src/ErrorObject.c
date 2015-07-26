#include <malloc.h>
#include "ErrorObject.h"
#include "CException.h"


/**
 *  ThrowError generate a errorObj that stored the detail of errObj 
 *  This function stored error details as shown
 *         __________
 *        [ errorMsg ]
 *        [errorCode ] 
 *
 *
 *  Eg,
 *  Calling function by
 *  ThrowError("This operator is undefined!", ERR_UNDEFINED_OPERATOR)
 *		   _____________________________
 *        ["This operator is undefined!"]
 *        [   ERR_UNDEFINED_OPERATOR    ]
 */
void ThrowError(char *message, ErrorCode errCode){
  ErrorObject* errObj = malloc(sizeof(ErrorObject));
  errObj->errorMsg = message;
  errObj->errorCode = errCode;
  Throw(errObj);
}

void freeError(ErrorObject *errObj){
  free(errObj);
}
