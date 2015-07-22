#ifndef tokenExtend_H
#define tokenExtend_H
#include "Token.h"
#include "ErrorObject.h"
#include <stdint.h>



typedef struct OperatorAttributes_t OperatorAttributes;
struct OperatorAttributes_t{
  uint32_t bindingPower;
  Arity arity;
  Token* (*nud)(Token*);
  Token* (*led)(Token*);
  Token* (*extend)(Token *token, OperatorAttributes *attributes);
};

Token* extendSingleCharacterOperator(Token *thisOpr, OperatorAttributes *attr);
Token* extendDoubleCharacterOperator(Token *thisOpr, OperatorAttributes *attr);
Token* extendTripleCharacterOperator(Token *thisOpr, OperatorAttributes *attr);
Token* extendQuadrupleCharacterOperator(Token *thisOpr, OperatorAttributes *attr);
Token* infixNud(Token* myself);
Token* infixLed(Token* myself);

#endif // tokenExtend_H