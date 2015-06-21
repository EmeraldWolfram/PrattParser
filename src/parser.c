#include "parser.h"
#include "Token.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Token* parser(int prevBindingPower){
  
  OperatorToken* nextOprToken = malloc(sizeof(OperatorToken) + 2*(sizeof(Token*)));
  IntegerToken* nextIntToken  = malloc(sizeof(IntegerToken));
  OperatorToken* currentToken = malloc(sizeof(OperatorToken));
  Token* nextToken = malloc(sizeof(Token));
  
  do{
    nextToken = peepToken();
    if(nextToken->type == TOKEN_INTEGER_TYPE){
      nextIntToken = (IntegerToken*)getToken();
      nextOprToken = (OperatorToken*)peepToken();
    }
    else{
      nextOprToken = (OperatorToken*)nextToken;
    }
    currentToken = (OperatorToken*)peepToken();
    if((nextOprToken->bindingPower)> prevBindingPower){
      nextOprToken = (OperatorToken*)getToken();
      nextOprToken->token[0] = (Token*)nextIntToken;
      nextOprToken->token[1] = parser(nextOprToken->bindingPower);
    }
    else{
      return (Token*)nextIntToken;
    }
  }while(strcmp(currentToken->symbol, "$") != 0);
  return (Token*)nextOprToken->token[0];
}