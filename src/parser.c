#include "parser.h"
#include "Token.h"
#include "tokenExtend.h"
#include "CException.h"
#include "getToken.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Token* parser(int prevBindingPower){
  OperatorToken* nextOprToken = malloc(sizeof(OperatorToken) + 2*(sizeof(Token*)));
  IntegerToken* nextIntToken  = malloc(sizeof(IntegerToken));
  OperatorToken* currentToken = malloc(sizeof(OperatorToken));
  Token* nextToken            = malloc(sizeof(Token));
  
  nextToken       = getToken();
  nextToken->nud  = infixNud;
  nextIntToken    = (IntegerToken*)nextToken->nud(nextToken);
  
  nextToken       = peepToken();
  nextToken->led  = infixLed;
  nextOprToken    = (OperatorToken*)nextToken->led(nextToken);
  
  if((nextOprToken->bindingPower)> prevBindingPower){
    nextOprToken = (OperatorToken*)getToken();
    nextOprToken->token[0] = (Token*)nextIntToken;
    if(nextOprToken->arity != POSTFIX)
      nextOprToken->token[1] = parser(nextOprToken->bindingPower);
  }
  else
    return (Token*)nextIntToken;
  
  do{  
    currentToken = (OperatorToken*)peepToken();
    if(currentToken->bindingPower > prevBindingPower){
      currentToken = (OperatorToken*)getToken();
      currentToken->token[0] = (Token*)nextOprToken;
      currentToken->token[1] = parser(currentToken->bindingPower);
      nextOprToken = currentToken;
    }
  }while(strcmp(currentToken->symbol, "$") != 0 && prevBindingPower == 0);
  return (Token*)nextOprToken;
}