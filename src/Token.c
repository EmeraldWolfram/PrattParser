#include "Token.h"
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



Token *createOperatorToken(char *symbol, Arity AR){
  int i;
  if(AR == INFIX)
    i = 2;
  else
    i = 1;
  
  OperatorToken* op = malloc(sizeof(OperatorToken)+ sizeof(Token*) * i);
  
  if(*symbol == '+')
    op->bindingPower = ADD;
  else if(*symbol == '*')
    op->bindingPower = MUL;
  else if(*symbol == '/')
    op->bindingPower = DIV;
  else if(*symbol == '-')
    op->bindingPower = SUB;
  else
    op->bindingPower = 0;
  
  op->type = TOKEN_OPERATOR_TYPE;
  op->symbol = symbol;
  op->arity = AR;

  return (Token*)op;
}

Token *createIntegerToken(int value){
  IntegerToken* intTk = malloc(sizeof(IntegerToken));
    intTk->type = TOKEN_INTEGER_TYPE;
    intTk->value = value;
  return (Token*)intTk;  
}