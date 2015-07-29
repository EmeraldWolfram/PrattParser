#include "execute.h"
#include "parser.h"
#include "Token.h"
#include "tokenExtend.h"
#include "CException.h"
#include "getToken.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* executeSingle(Token* token){
  char* mainStr     = malloc(sizeof(char));
  char* minorStr    = malloc(sizeof(char));
  Token* minorToken  = ((OperatorToken*)token)->token[0];
  minorStr          = minorToken->execute(minorToken);  
  sprintf(mainStr, "(%s%s)", ((OperatorToken*)token)->symbol, minorStr);
  return mainStr;
}

char* executeDouble(Token* token){
  char* mainStr       = malloc(sizeof(char));
  char* leftMinorStr  = malloc(sizeof(char));
  char* rightMinorStr = malloc(sizeof(char));
  
  Token* leftMinorToken   = ((OperatorToken*)token)->token[0];
  Token* rightMinorToken  = ((OperatorToken*)token)->token[1];
  
  leftMinorStr            = leftMinorToken->execute(leftMinorToken);
  rightMinorStr           = rightMinorToken->execute(rightMinorToken);
  
  sprintf(mainStr, "(%s %s %s)", leftMinorStr, ((OperatorToken*)token)->symbol, rightMinorStr);
  
  return mainStr;
}

char* executeExpression(Token* token){
  char* myString = malloc(sizeof(char));
  switch(token->type){
    case TOKEN_INTEGER_TYPE:
      sprintf(myString, "%d", ((IntegerToken*)token)->value);
    case TOKEN_FLOAT_TYPE:
      sprintf(myString, "%f", ((FloatToken*)token)->value);
    case TOKEN_STRING_TYPE:
      sprintf(myString, "%s", ((StringToken*)token)->name);
    case TOKEN_IDENTIFIER_TYPE:
      sprintf(myString, "%s", ((StringToken*)token)->name);
  }
  return myString;
}