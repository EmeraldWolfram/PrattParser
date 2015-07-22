#include "tokenExtend.h"
#include "Token.h"
#include "ErrorObject.h"
#include "CException.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// OperatorAttributes operatorAttributesTable[] = {
  // ['<'] = {30, INFIX, infixNud, infixLed, extendQuadrupleCharacterOperator},
  // ['>'] = {30, INFIX, infixNud, infixLed, extendQuadrupleCharacterOperator},
  // ['+'] = {50, PREFIX, infixNud, infixLed, extendTripleCharacterOperator},
  // ['-'] = {50, PREFIX, infixNud, infixLed, extendTripleCharacterOperator},
  // ['&'] = {10, PREFIX, infixNud, infixLed, extendTripleCharacterOperator},
  // ['|'] = { 8, INFIX, infixNud, infixLed, extendTripleCharacterOperator},
  // ['*'] = {60, PREFIX, infixNud, infixLed, extendDoubleCharacterOperator},
  // ['/'] = {60, INFIX, infixNud, infixLed, extendDoubleCharacterOperator},
  // ['%'] = {60, INFIX, infixNud, infixLed, extendDoubleCharacterOperator},
  // ['^'] = { 9, INFIX, infixNud, infixLed, extendDoubleCharacterOperator},
  // ['!'] = {70, INFIX, infixNud, infixLed, extendDoubleCharacterOperator},
  // ['='] = { 5, INFIX, infixNud, infixLed, extendDoubleCharacterOperator},
  // ['~'] = {70, PREFIX, infixNud, infixLed, extendSingleCharacterOperator},
  // ['('] = {70, PREFIX, infixNud, infixLed, extendSingleCharacterOperator},
  // ['['] = {70, PREFIX, infixNud, infixLed, extendSingleCharacterOperator}
// };

Token* infixNud(Token* myself){

  ErrorObject *err;
  Try{
    char* symbol = ((OperatorToken*)myself)->symbol;
    if(myself->type == TOKEN_OPERATOR_TYPE){
      if(strcmp(symbol,"-") == 0 || strcmp(symbol,"+") == 0 || strcmp(symbol,"++") == 0 || strcmp(symbol,"--") == 0){
        ((OperatorToken*)myself)->token[0]  = parser(100);
        ((OperatorToken*)myself)->arity     = PREFIX;
      }
      else
        ThrowError("This is not a legal PREFIX operator!", ERR_ILLEGAL_PREFIX);
    }
    return myself;
  }Catch(err) {
    printf("%s\n", err->errorMsg);
  }
}

Token* infixLed(Token* myself){
  ErrorObject *err;
  char* symbol = ((OperatorToken*)myself)->symbol;
  Try{  
    if(myself->type != TOKEN_OPERATOR_TYPE)
      ThrowError("This operator is undefined!", ERR_UNDEFINED_OPERATOR);
    else if(strcmp(symbol,"++") == 0 || strcmp(symbol,"--") == 0)
      ((OperatorToken*)myself)->arity = POSTFIX;
    
    return myself;
  }Catch(err) {
    printf("%s\n", err->errorMsg);
  }
}


Token* extendSingleCharacterOperator(Token* thisOpr, OperatorAttributes *attr){
  ((OperatorToken*)thisOpr)->bindingPower  = attr->bindingPower;
  ((OperatorToken*)thisOpr)->arity         = attr->arity;
  ((OperatorToken*)thisOpr)->nud           = attr->nud;
  ((OperatorToken*)thisOpr)->led           = attr->led;
  
  return thisOpr;
}

Token* extendDoubleCharacterOperator(Token *thisOpr, OperatorAttributes *attr){ 
  ErrorObject *err;
  Try{  
    ((OperatorToken*)thisOpr)->nud           = attr->nud;
    ((OperatorToken*)thisOpr)->led           = attr->led;
    
    if(((OperatorToken*)thisOpr)->symbol[1] == '='){
      ((OperatorToken*)thisOpr)->bindingPower  = 5;
      ((OperatorToken*)thisOpr)->arity         = INFIX;
    }
    else if(((OperatorToken*)thisOpr)->symbol[1] == 0){
      ((OperatorToken*)thisOpr)->bindingPower  = attr->bindingPower;
      ((OperatorToken*)thisOpr)->arity         = attr->arity;
    }
    else
      ThrowError("This operator is undefined!", ERR_UNDEFINED_OPERATOR); //When not "*=","*" etc, then throw error.
  
    if(strcmp(((OperatorToken*)thisOpr)->symbol, "==") == 0)
      ((OperatorToken*)thisOpr)->bindingPower = 20;
    
    
    return thisOpr;
  }Catch(err) {
    printf("%s\n", err->errorMsg);
  }  
}

Token* extendTripleCharacterOperator(Token *thisOpr, OperatorAttributes *attr){
  ((OperatorToken*)thisOpr)->nud           = attr->nud;
  ((OperatorToken*)thisOpr)->led           = attr->led;
  ((OperatorToken*)thisOpr)->arity        = attr->arity;
  
  ErrorObject *err;
  Try{ 
    if(((OperatorToken*)thisOpr)->symbol[1] == '='){
      ((OperatorToken*)thisOpr)->bindingPower = 5;
      ((OperatorToken*)thisOpr)->arity        = INFIX;
    }
    else if(((OperatorToken*)thisOpr)->symbol[1] == ((OperatorToken*)thisOpr)->symbol[0])
      ((OperatorToken*)thisOpr)->bindingPower = 70;
    else if(((OperatorToken*)thisOpr)->symbol[1] == 0)
      ((OperatorToken*)thisOpr)->bindingPower = attr->bindingPower;
    else
      ThrowError("This operator is undefined!", ERR_UNDEFINED_OPERATOR); //When not "+=","++","+" etc, then throw error.
    
    if(strcmp(((OperatorToken*)thisOpr)->symbol, "&&") == 0){
      ((OperatorToken*)thisOpr)->bindingPower = 7;
      ((OperatorToken*)thisOpr)->arity         = INFIX;
      }
    if(strcmp(((OperatorToken*)thisOpr)->symbol, "||") == 0){
      ((OperatorToken*)thisOpr)->bindingPower = 6;
    }
    
    return thisOpr;
  }Catch(err) {
    printf("%s\n", err->errorMsg);
  }  
}

Token* extendQuadrupleCharacterOperator(Token *thisOpr, OperatorAttributes *attr){
  ((OperatorToken*)thisOpr)->arity         = attr->arity;
  ((OperatorToken*)thisOpr)->nud           = attr->nud;
  ((OperatorToken*)thisOpr)->led           = attr->led;
  ErrorObject *err;
  Try{   
    if(((OperatorToken*)thisOpr)->symbol[1] == 0)
      ((OperatorToken*)thisOpr)->bindingPower = attr->bindingPower;  
    else if(((OperatorToken*)thisOpr)->symbol[1] == '=')
      ((OperatorToken*)thisOpr)->bindingPower = attr->bindingPower;
    else if(((OperatorToken*)thisOpr)->symbol[1] == ((OperatorToken*)thisOpr)->symbol[0]){
      if(((OperatorToken*)thisOpr)->symbol[2] == 0)
        ((OperatorToken*)thisOpr)->bindingPower = 40;
      else if(((OperatorToken*)thisOpr)->symbol[2] == '=')
        ((OperatorToken*)thisOpr)->bindingPower = 5;
      else
        ThrowError("This operator is undefined!", ERR_UNDEFINED_OPERATOR); //When not "<<","<<=", then throw error.
    }
    else
      ThrowError("This operator is undefined!", ERR_UNDEFINED_OPERATOR); //When not "<","<=", then throw error.
    return thisOpr;
  }Catch(err) {
    printf("%s\n", err->errorMsg);
  } 
}
