#ifndef tokenExtend_H
#define tokenExtend_H
#include "Token.h"
#include "ErrorObject.h"
#include "execute.h"
#include "CException.h"
#include <stdint.h>

typedef struct Attributes_t Attributes;
struct Attributes_t{
  uint32_t bindingPower;
  Arity arity;
  Token* (*nud)(Token*);
  Token* (*led)(Token*);
  Token* (*extend)(Token *token, Attributes *attributes);
  char* (*execute)(Token *token);
};

Token* extendSingleCharacterOperator(Token *thisOpr, Attributes *attr);
Token* extendDoubleCharacterOperator(Token *thisOpr, Attributes *attr);
Token* extendTripleCharacterOperator(Token *thisOpr, Attributes *attr);
Token* extendQuadrupleCharacterOperator(Token *thisOpr, Attributes *attr);
Token* extendExpression(Token *thisExpression, Attributes *attr);

Token* extendErrorOperator(Token *thisOpr, Attributes *attr);
Token* extendCharacterErrorOperator(Token *thisOpr, Attributes *attr);
Token* extendIntegerErrorOperator(Token *thisOpr, Attributes *attr);

Token* prefixNud(Token* myself);
Token* expressionNud(Token* myself);
Token* errorNud(Token* myself);
Token* infixLed(Token* myself);
Token* errorLed(Token* myself);


#endif // tokenExtend_H