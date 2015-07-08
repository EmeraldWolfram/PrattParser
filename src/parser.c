#include "parser.h"
#include "Token.h"
#include "getToken.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Token* parser(int prevBindingPower){
  OperatorToken* nextOprToken = malloc(sizeof(OperatorToken) + 2*(sizeof(Token*)));
  IntegerToken* nextIntToken  = malloc(sizeof(IntegerToken));
  OperatorToken* currentToken = malloc(sizeof(OperatorToken));
  Token* nextToken = malloc(sizeof(Token));
  
  nextToken = getToken();
  nextIntToken = (IntegerToken*)infixNud(nextToken);
  nextToken = peepToken();
  nextOprToken = (OperatorToken*)infixLed(nextToken);
  printf("In parser(%d)\n", prevBindingPower);
  
  if((nextOprToken->bindingPower)> prevBindingPower){
    nextOprToken = (OperatorToken*)getToken();
    nextOprToken->token[0] = (Token*)nextIntToken;
    nextOprToken->token[1] = parser(nextOprToken->bindingPower);
    // printf("\nReturn to parser(%d)", prevBindingPower);
  }
  else
    return (Token*)nextIntToken;
  
  do{  
    currentToken = (OperatorToken*)peepToken();
    if(currentToken->bindingPower > prevBindingPower){
      currentToken = (OperatorToken*)getToken();
      currentToken->token[0] = (Token*)nextOprToken;
      currentToken->token[1] = parser(currentToken->bindingPower);
      // printf("\n*Return to parser(%d)", prevBindingPower);
      nextOprToken = currentToken;
    }
  }while(strcmp(currentToken->symbol, "$") != 0 && prevBindingPower == 0);
  return (Token*)nextOprToken;
}