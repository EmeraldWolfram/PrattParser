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
  // printf("Single get called\n");
  char* mainStr     = malloc(sizeof(char)*100);
  char* minorStr    = malloc(sizeof(char)*50);
  Token* minorToken  = ((OperatorToken*)token)->token[0];
  minorStr          = minorToken->execute(minorToken);  
  int n;
  n = sprintf(mainStr, "(%s%s)", ((OperatorToken*)token)->symbol, minorStr);
  return mainStr;
}

char* executeDouble(Token* token){
  // printf("Double get called\n");
  char* mainStr       = malloc(sizeof(char)*200);
  char* leftMinorStr  = malloc(sizeof(char)*50);
  char* rightMinorStr = malloc(sizeof(char)*50);
  int n;
  Token* leftMinorToken   = malloc(sizeof(Token));
  leftMinorToken  = ((OperatorToken*)token)->token[0];
  Token* rightMinorToken  = malloc(sizeof(Token));
  rightMinorToken = ((OperatorToken*)token)->token[1];
  
  leftMinorStr    = leftMinorToken->execute(leftMinorToken);
  rightMinorStr   = rightMinorToken->execute(rightMinorToken);
  n = sprintf(mainStr, "(%s %s %s)", leftMinorStr, ((OperatorToken*)token)->symbol, rightMinorStr);

  return mainStr;
}

char* executeExpression(Token* token){
  // printf("Express get called\n");
  Token* executeToken = malloc(sizeof(StringToken));
  char* myString      = malloc(sizeof(char)*50);
  executeToken = token;
  int i;

  i = sprintf(myString, "%d", ((IntegerToken*)executeToken)->value);

  return myString;
}