#include "parser.h"
#include "Token.h"
#include <stdlib.h>
#include <stdio.h>

Token* parser(int prevBindingPower){
  
  OperatorToken* nextOprToken = malloc(sizeof(OperatorToken*) + 2*(sizeof(Token*)));
  IntegerToken* nextIntToken  = malloc(sizeof(IntegerToken*));
  
  nextIntToken = (IntegerToken*)getToken();
  nextOprToken = (OperatorToken*)getToken();

  if((nextOprToken->bindingPower)> prevBindingPower){
    nextOprToken->token[0] = (Token*)nextIntToken;
    nextOprToken->token[1] = parser(nextOprToken->bindingPower);
    //should run operation and return the value as an IntegerToken*
  }
  else{
    return (Token*)nextIntToken;  //Yen, this is the part, I will chop the list if like this! Make it continue
  }
    
  
  
  return (Token*)nextOprToken;
}