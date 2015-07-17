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

// OperatorAttributes_t operatorAttributesTable[] = {
  // ['+'] = {20, INFIX, infixNud, infixLed},
  // ['*'] = {30, INFIX, infixNud, infixLed},
  // ['-'] = {20, INFIX, infixNud, infixLed},
  // ['/'] = {30, INFIX, infixNud, infixLed}
// };

Token* infixLed(Token* myself);
Token* infixNud(Token* myself);
void extendSingleCharacterOperator();
void extendMultiCharacterOperator();

#endif // tokenExtend_H


// Token* nextToken = malloc(sizeof(Token));
// nextToken->extend = extendSingleCharacterOperator(nextToken, operatorAttributesTable['++']);