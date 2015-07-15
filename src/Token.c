#include "Token.h"
#include "parser.h"
#include "ErrorObject.h"
#include "CException.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 *  createIntegerToken generate an Integer token that stored an int value 
 *  This function take an integer argument and stored it
 *         _______
 *        [INTEGER]
 *        [ VALUE ] 
 *
 *
 *  Eg,
 *  Calling function by
 *  Token = createIntegerToken(3);
 *  shall return this token
 *         _______
 *        [INTEGER]
 *        [  3    ]
 ********************************************************************************
 *  createOperatorToken generate an Operator token that stored the operator symbol,
 *  arity of the operator and pointer/pointers that will point to another Token  
 *  This function take a character argument and an arity argument.
 *         _______
 *        [OPERATOR]
 *        [ SYMBOL ]
 *        [ ARITY  ]
 *        [ptr][ptr]
 *
 *
 *  Eg,
 *  Calling function by
 *  Token = createOperatorToken("*", INFIX)
 *  shall return this token
 *         _______
 *        [OPERATOR]
 *        [   *    ]
 *        [  INFIX ]
 *        [ptr][ptr]
 *
 *  ptr in the diagram means pointer which store an address of a Token
 *
 */

Token* infixNud(Token* myself){

  ErrorObject *err;
  Try{
    char* symbol = ((OperatorToken*)myself)->symbol;
    if(myself->type == TOKEN_OPERATOR_TYPE){
      if(strcmp(symbol,"-") == 0 || strcmp(symbol,"+") == 0 || strcmp(symbol,"++") == 0 || strcmp(symbol,"--") == 0)
        ((OperatorToken*)myself)->token[0] = parser(100);
      else
        ThrowError("This is not a legal PREFIX operator!", ERR_ILLEGAL_PREFIX);
    }
    return myself;
  }Catch(err) {
    printf("%c/n", err->errorMsg);
  }
}

Token* infixLed(Token* myself){
  ErrorObject *err;
  Try{  
    if(myself->type != TOKEN_OPERATOR_TYPE)
      ThrowError("This operator is undefined!", ERR_UNDEFINED_OPERATOR);
    return myself;
  }Catch(err) {
    printf("%c/n", err->errorMsg);
  }
}


Token *createOperatorToken(char *symbol, Arity AR){
  int i;
  if(AR == INFIX)
    i = 2;
  else
    i = 1;
  
  OperatorToken* op = malloc(sizeof(OperatorToken)+ sizeof(Token*) * i);
  
  if(strcmp(symbol,"+") == 0 || strcmp(symbol,"-") == 0)
    op->bindingPower  = 20;
  else if(strcmp(symbol,"*") == 0 || strcmp(symbol,"/") == 0)
    op->bindingPower  = 30;
  else if(strcmp(symbol,"++") == 0 || strcmp(symbol,"--") == 0)
    op->bindingPower  = 60;
  else
    op->bindingPower  = 0;
  
  op->type    = TOKEN_OPERATOR_TYPE;
  op->symbol  = symbol;
  op->arity   = AR;

  return (Token*)op;
}

Token *createIntegerToken(int value){
  IntegerToken* intTk = malloc(sizeof(IntegerToken));
    intTk->type   = TOKEN_INTEGER_TYPE;
    intTk->value  = value;
  return (Token*)intTk;  
}