#include "parser.h"
#include "Token.h"
#include "tokenExtend.h"
#include "CException.h"
#include "getToken.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern Attributes operatorAttributesTable[];
extern Attributes tokenTypeAttributesTable[];

/****************************************************************************************
 * 	This is the main function of the Pratt Parser.
 *  
 */
Token* parser(int prevBindingPower){
  Token* nextToken            = malloc(sizeof(Token));
  IntegerToken* nextIntToken  = malloc(sizeof(IntegerToken));
  OperatorToken* nextOprToken = malloc(sizeof(OperatorToken) + 2*(sizeof(Token*)));
  OperatorToken* currentToken = malloc(sizeof(OperatorToken) + 2*(sizeof(Token*)));
  Attributes* attr            = malloc(sizeof(Attributes));
  
  nextToken     = getToken();
  attr          = &tokenTypeAttributesTable[nextToken->type];
  nextIntToken  = (IntegerToken*)attr->extend(nextToken, attr);
  nextIntToken  = (IntegerToken*)nextToken->nud((Token*)nextIntToken);
  
  nextToken     = peepToken();
  attr          = &tokenTypeAttributesTable[nextToken->type];
  nextOprToken  = (OperatorToken*)attr->extend(nextToken, attr);
  nextOprToken  = (OperatorToken*)nextToken->led((Token*)nextOprToken);
  
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
    attr         = &tokenTypeAttributesTable[currentToken->type];
    currentToken = (OperatorToken*)attr->extend((Token*)currentToken, attr);
    currentToken = (OperatorToken*)currentToken->led((Token*)currentToken);
    if(strcmp(currentToken->symbol, ")") == 0 \
    || strcmp(currentToken->symbol, "]") == 0){
      currentToken = (OperatorToken*)getToken();
    }
    if(currentToken->bindingPower > prevBindingPower){
      currentToken = (OperatorToken*)getToken();
      currentToken->token[0] = (Token*)nextOprToken;
      currentToken->token[1] = parser(currentToken->bindingPower);
      nextOprToken = currentToken;
    }
  }while(strcmp(currentToken->symbol, "$") != 0 \
      && strcmp(currentToken->symbol, ")") != 0 \
      && strcmp(currentToken->symbol, "]") != 0 \
      && prevBindingPower == 0);
  return (Token*)nextOprToken;
}