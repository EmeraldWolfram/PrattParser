#include "tokenExtend.h"
#include "Token.h"
#include "ErrorObject.h"
#include "CException.h"
#include "parser.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

OperatorAttributes operatorAttributesTable[] = {
  ['<'] = {30, INFIX,  infixNud, infixLed, extendQuadrupleCharacterOperator},
  ['>'] = {30, INFIX,  infixNud, infixLed, extendQuadrupleCharacterOperator},
  ['+'] = {50, PREFIX, infixNud, infixLed, extendTripleCharacterOperator},
  ['-'] = {50, PREFIX, infixNud, infixLed, extendTripleCharacterOperator},
  ['&'] = {10, PREFIX, infixNud, infixLed, extendTripleCharacterOperator},
  ['|'] = { 8, INFIX,  infixNud, infixLed, extendTripleCharacterOperator},
  ['*'] = {60, PREFIX, infixNud, infixLed, extendDoubleCharacterOperator},
  ['/'] = {60, INFIX,  infixNud, infixLed, extendDoubleCharacterOperator},
  ['%'] = {60, INFIX,  infixNud, infixLed, extendDoubleCharacterOperator},
  ['^'] = { 9, INFIX,  infixNud, infixLed, extendDoubleCharacterOperator},
  ['!'] = {70, INFIX,  infixNud, infixLed, extendDoubleCharacterOperator},
  ['='] = { 5, INFIX,  infixNud, infixLed, extendDoubleCharacterOperator},
  ['~'] = {70, PREFIX, infixNud, infixLed, extendSingleCharacterOperator},
  ['('] = {1,  PREFIX, infixNud, infixLed, extendSingleCharacterOperator},
  [')'] = {1,  NOFIX,  infixNud, infixLed, extendSingleCharacterOperator},
  ['['] = {1,  PREFIX, infixNud, infixLed, extendSingleCharacterOperator},
  [']'] = {1,  NOFIX,  infixNud, infixLed, extendSingleCharacterOperator},
  ['$'] = {0,  NOFIX,  infixNud, infixLed, extendSingleCharacterOperator},
  ['#'] = {1,  NOFIX,  infixNud, infixLed, extendErrorOperator},
  ['{'] = {1,  NOFIX,  infixNud, infixLed, extendErrorOperator},
  ['}'] = {1,  NOFIX,  infixNud, infixLed, extendErrorOperator},
  ['@'] = {1,  NOFIX,  infixNud, infixLed, extendErrorOperator},
  ['?'] = {1,  NOFIX,  infixNud, infixLed, extendErrorOperator},
  ['.'] = {1,  NOFIX,  infixNud, infixLed, extendErrorOperator},
  [','] = {1,  NOFIX,  infixNud, infixLed, extendErrorOperator},
  [';'] = {1,  NOFIX,  infixNud, infixLed, extendErrorOperator},
  ['"'] = {1,  NOFIX,  infixNud, infixLed, extendErrorOperator},
  ['\'']= {1,  NOFIX,  infixNud, infixLed, extendErrorOperator},
  ['`'] = {1,  NOFIX,  infixNud, infixLed, extendErrorOperator},
  ['\\']= {1,  NOFIX,  infixNud, infixLed, extendErrorOperator},
  ['a' ... 'z'] = {1,  NOFIX,  infixNud, infixLed, extendCharacterErrorOperator},
  ['A' ... 'Z'] = {1,  NOFIX,  infixNud, infixLed, extendCharacterErrorOperator},
  [48  ...  57] = {1,  NOFIX,  infixNud, infixLed, extendIntegerErrorOperator}
};

Token* infixNud(Token* myself){
    assert(myself != NULL);
    if(myself->type == TOKEN_OPERATOR_TYPE){
      int thisSymbol            = *(char*)((OperatorToken*)myself)->symbol;
      OperatorAttributes* attr  = &operatorAttributesTable[thisSymbol];
      
      myself  = attr->extend(myself, attr);
      if(attr->arity == PREFIX){
        if(strcmp(((OperatorToken*)myself)->symbol,"(") != 0)
          ((OperatorToken*)myself)->token[0] = parser(100);
        else
          ((OperatorToken*)myself)->token[0] = parser(0);
      }
      else 
        ThrowError("This is an illegal prefix!", ERR_ILLEGAL_PREFIX);
    }
    return myself;
}

Token* infixLed(Token* myself){
  assert(myself != NULL);
  char* symbol = ((OperatorToken*)myself)->symbol;
  if(myself->type != TOKEN_OPERATOR_TYPE)
    ThrowError("Expected operator token but obtained identifier token!", ERR_UNEXPECTED_IDENTIFIER);
  else{
    int thisSymbol = *(char*)((OperatorToken*)myself)->symbol;
    OperatorAttributes* attr = &operatorAttributesTable[thisSymbol];
    myself = attr->extend(myself, attr);
    ((OperatorToken*)myself)->arity = INFIX;
    if(strcmp(symbol,"++") == 0 || strcmp(symbol,"--") == 0)
      ((OperatorToken*)myself)->arity = POSTFIX;
  }
  return myself;
}


Token* extendSingleCharacterOperator(Token* thisOpr, OperatorAttributes *attr){
  if(((OperatorToken*)thisOpr)->symbol[1] == 0){
    ((OperatorToken*)thisOpr)->bindingPower  = attr->bindingPower;
    ((OperatorToken*)thisOpr)->arity         = attr->arity;
    ((OperatorToken*)thisOpr)->nud           = attr->nud;
    ((OperatorToken*)thisOpr)->led           = attr->led;
  }
  else
    ThrowError("This operator is undefined!", ERR_UNDEFINED_OPERATOR);  //Throw error such as "~X", where X can be any character
  return thisOpr;
}

Token* extendDoubleCharacterOperator(Token *thisOpr, OperatorAttributes *attr){ 
  ((OperatorToken*)thisOpr)->nud           = attr->nud;
  ((OperatorToken*)thisOpr)->led           = attr->led;
    
  if(((OperatorToken*)thisOpr)->symbol[1] == 0){
    ((OperatorToken*)thisOpr)->bindingPower  = attr->bindingPower;
    ((OperatorToken*)thisOpr)->arity         = attr->arity;
  }
  else if(((OperatorToken*)thisOpr)->symbol[1] == '='){
    if(((OperatorToken*)thisOpr)->symbol[2] == 0){
      ((OperatorToken*)thisOpr)->bindingPower  = 5;
      ((OperatorToken*)thisOpr)->arity         = INFIX;
    }
    else
      ThrowError("This operator is undefined!", ERR_UNDEFINED_OPERATOR); //Throw error such as "*=X" detected, where X can be any character
  }
  else
    ThrowError("This operator is undefined!", ERR_UNDEFINED_OPERATOR); //Throw error when "*X" detected, where X can be any character
  
  if(strcmp(((OperatorToken*)thisOpr)->symbol, "==") == 0)
    ((OperatorToken*)thisOpr)->bindingPower = 20;  
    
  return thisOpr;
}

Token* extendTripleCharacterOperator(Token *thisOpr, OperatorAttributes *attr){
  ((OperatorToken*)thisOpr)->nud           = attr->nud;
  ((OperatorToken*)thisOpr)->led           = attr->led;
  ((OperatorToken*)thisOpr)->arity         = attr->arity;
  
  if(((OperatorToken*)thisOpr)->symbol[1] == '='){
    if(((OperatorToken*)thisOpr)->symbol[2] == 0){
      ((OperatorToken*)thisOpr)->bindingPower = 5;
      ((OperatorToken*)thisOpr)->arity        = INFIX;
    }
    else
      ThrowError("This operator is undefined!", ERR_UNDEFINED_OPERATOR);      //Throw error when "+=X" detected, where X can be any character
  }
  else if(((OperatorToken*)thisOpr)->symbol[1] == ((OperatorToken*)thisOpr)->symbol[0]){
    if(((OperatorToken*)thisOpr)->symbol[2] == 0)
      ((OperatorToken*)thisOpr)->bindingPower = 70;
    else
      ThrowError("This operator is undefined!", ERR_UNDEFINED_OPERATOR);      //Throw error when "++X" detected, where X can be any character
  }
  else if(((OperatorToken*)thisOpr)->symbol[1] == 0)
    ((OperatorToken*)thisOpr)->bindingPower = attr->bindingPower;
  else
    ThrowError("This operator is undefined!", ERR_UNDEFINED_OPERATOR);        //Throw error when "+X" detected, where X can be any character other than '=' or itself
   
  if(strcmp(((OperatorToken*)thisOpr)->symbol, "&&") == 0){
    ((OperatorToken*)thisOpr)->bindingPower = 7;
    ((OperatorToken*)thisOpr)->arity         = INFIX;
    }
  if(strcmp(((OperatorToken*)thisOpr)->symbol, "||") == 0){
    ((OperatorToken*)thisOpr)->bindingPower = 6;
  }
    
  return thisOpr;
}

Token* extendQuadrupleCharacterOperator(Token *thisOpr, OperatorAttributes *attr){
  ((OperatorToken*)thisOpr)->arity         = attr->arity;
  ((OperatorToken*)thisOpr)->nud           = attr->nud;
  ((OperatorToken*)thisOpr)->led           = attr->led;

    if(((OperatorToken*)thisOpr)->symbol[1] == 0)
      ((OperatorToken*)thisOpr)->bindingPower = attr->bindingPower;  
    else if(((OperatorToken*)thisOpr)->symbol[1] == '='){
      if(((OperatorToken*)thisOpr)->symbol[2] == 0)
        ((OperatorToken*)thisOpr)->bindingPower = attr->bindingPower;
      else
        ThrowError("This operator is undefined!", ERR_UNDEFINED_OPERATOR);        //Throw error when ">=X" detected, where X can be any character
    }
    else if(((OperatorToken*)thisOpr)->symbol[1] == ((OperatorToken*)thisOpr)->symbol[0]){
      if(((OperatorToken*)thisOpr)->symbol[2] == 0)
        ((OperatorToken*)thisOpr)->bindingPower = 40;
      else if(((OperatorToken*)thisOpr)->symbol[2] == '='){
        if(((OperatorToken*)thisOpr)->symbol[3] == 0)
          ((OperatorToken*)thisOpr)->bindingPower = 5;
        else
          ThrowError("This operator is undefined!", ERR_UNDEFINED_OPERATOR);      //Throw error when ">>=X" detected, where X can be any character
      }
      else
        ThrowError("This operator is undefined!", ERR_UNDEFINED_OPERATOR);        //Throw error when ">>X" detected, where X can be any character except '='
    }
    else
      ThrowError("This operator is undefined!", ERR_UNDEFINED_OPERATOR);          //Throw error when ">X" detected, where X can be any character except '=' and itself
    return thisOpr;
}

Token* extendCharacterErrorOperator(Token *thisOpr, OperatorAttributes *attr){
  ThrowError("This is illegal character!", ERR_ILLEGAL_CHARACTER);
}

Token* extendErrorOperator(Token *thisOpr, OperatorAttributes *attr){
  ThrowError("This operator is undefined!", ERR_UNDEFINED_OPERATOR);
}

Token* extendIntegerErrorOperator(Token *thisOpr, OperatorAttributes *attr){
  ThrowError("Integer unexpected to be here!", ERR_UNEXPECTED_INTEGER);
}


