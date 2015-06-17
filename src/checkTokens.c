#include "checkTokens.h"
#include "AddToken.h"
#include "Token.h"
#include <stdlib.h>
#include <stdio.h>

Token* checkTokens(OperatorToken* leftOperator, OperatorToken* rightOperator){
  int left = leftOperator->bindingPower;
  int right = rightOperator->bindingPower;
  
  if(right > left)
    return (Token*) rightOperator;
  else
    return (Token*) leftOperator;
}
