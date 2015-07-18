#include "tokenExtend.h"
#include "Token.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

OperatorAttributes operatorAttributesTable[] = {
  ['<'] = {30, INFIX, infixNud, infixLed, extendQuadrupleCharacterOperator},
  ['>'] = {30, INFIX, infixNud, infixLed, extendQuadrupleCharacterOperator},
  ['+'] = {50, INFIX, infixNud, infixLed, extendTripleCharacterOperator},
  ['-'] = {50, INFIX, infixNud, infixLed, extendTripleCharacterOperator},
  ['&'] = {10, INFIX, infixNud, infixLed, extendTripleCharacterOperator},
  ['|'] = { 8, INFIX, infixNud, infixLed, extendTripleCharacterOperator},
  ['*'] = {60, INFIX, infixNud, infixLed, extendDoubleCharacterOperator},
  ['/'] = {60, INFIX, infixNud, infixLed, extendDoubleCharacterOperator},
  ['%'] = {60, INFIX, infixNud, infixLed, extendDoubleCharacterOperator},
  ['^'] = {60, INFIX, infixNud, infixLed, extendDoubleCharacterOperator},
  ['!'] = {60, INFIX, infixNud, infixLed, extendDoubleCharacterOperator},
  ['='] = {20, INFIX, infixNud, infixLed, extendDoubleCharacterOperator},
  ['~'] = {70, PREFIX, infixNud, infixLed, extendSingleCharacterOperator},
  ['('] = {80, PREFIX, infixNud, infixLed, extendSingleCharacterOperator},
  ['['] = {80, PREFIX, infixNud, infixLed, extendSingleCharacterOperator}
};

Token* extendSingleCharacterOperator(){
  
}
Token* extendDoubleCharacterOperator(){
  
}
Token* extendTripleCharacterOperator(){
  
}
Token* extendQuadrupleCharacterOperator(){
  
}
