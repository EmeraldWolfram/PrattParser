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
  
  nextToken = (Token*)peepToken();
  // x = nextToken->nud(nextToken);
  // x 
  if(nextToken->type == TOKEN_INTEGER_TYPE){
    nextIntToken = (IntegerToken*)getToken();
    nextOprToken = (OperatorToken*)peepToken();
  }
  else{
    nextOprToken = (OperatorToken*)nextToken;
  }
  
  if((nextOprToken->bindingPower)> prevBindingPower){
    nextOprToken = (OperatorToken*)getToken();
    nextOprToken->token[0] = (Token*)nextIntToken;
    nextOprToken->token[1] = parser(nextOprToken->bindingPower);
  }
  else{
    return (Token*)nextIntToken;
  }
  currentToken = (OperatorToken*)peepToken();
  if(strcmp(currentToken->symbol, "$") != 0){
    if(currentToken->bindingPower > prevBindingPower){
      currentToken = (OperatorToken*)getToken();
      currentToken->token[0] = (Token*)nextOprToken;
      currentToken->token[1] = parser(currentToken->bindingPower);
      return (Token*)currentToken;
    }
  }
  return (Token*)nextOprToken;
}