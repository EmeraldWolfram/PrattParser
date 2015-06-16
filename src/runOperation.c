#include "runOperation.h"
#include "AddToken.h"
#include "Token.h"
#include <stdlib.h>
#include <stdio.h>

int runOperation(OperatorToken* operateThis){
  int left, right, answer;
  left  = ((IntegerToken*)operateThis->token[0])->value;
  right = ((IntegerToken*)operateThis->token[1])->value;
  char *symbol = operateThis->symbol;
  
    if(*symbol == '+')
      answer = left + right;
    else if(*symbol == '*')
      answer = left * right;
    else if(*symbol == '/')
      answer = left / right;
    else if(*symbol == '-')
      answer = left - right;
    else
      printf("Error operation!");
  
  return answer;
}