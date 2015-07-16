#include "tokenExtend.h"
#include "Token.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void extendSingleCharacterOperator(){
  
}

void extendMultiCharacterOperator(){
  
}

Token* infixNud(Token* myself){
  // char* symbol = ((OperatorToken*)myself)->symbol;
  // if(myself->type == TOKEN_OPERATOR_TYPE){
    // if(strcmp(symbol,"-") == 0 || strcmp(symbol,"+") == 0)
      // ((OperatorToken*)myself)->token[0] = parser(100);
    // else
      // printf("ERROR: PREFIX can only be '-' or '+'");//THROW ERROR
  // }
  return myself;
}
Token* infixLed(Token* myself){
  // if(myself->type != TOKEN_OPERATOR_TYPE)
    // printf("ERROR: (%d) is not an Operator!", ((IntegerToken*)myself)->value);//THROW ERROR
  // else
    
  return myself;
}