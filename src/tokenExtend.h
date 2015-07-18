#ifndef tokenExtend_H
#define tokenExtend_H
#include "Token.h"
#include <stdint.h>

typedef struct OperatorAttributes_t OperatorAttributes;
struct OperatorAttributes_t{
  uint32_t bindingPower;
  Arity arity;
  Token* (*nud)(Token*);
  Token* (*led)(Token*);
  Token* (*extend)(Token *token, OperatorAttributes *attributes);
};


//Token* infixLed(Token* myself);
//Token* infixNud(Token* myself);
Token* extendSingleCharacterOperator();
Token* extendDoubleCharacterOperator();
Token* extendTripleCharacterOperator();
Token* extendQuadrupleCharacterOperator();

#endif // tokenExtend_H