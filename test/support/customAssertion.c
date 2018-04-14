#include "customAssertion.h"
#include "unity.h"
#include "Token.h"
#include <string.h>


void customTestAssertTokenTree(Token* expectedOper, Token* token1, Token* token2, OperatorToken *actualOper, int lineNumber){
  customTestAssertOperator(expectedOper, actualOper, lineNumber);

//TEST for Token[0]
  if(token1->type == TOKEN_INTEGER_TYPE){
    if(((IntegerToken*)token1)->value != ((IntegerToken*)actualOper->token[0])->value)
      CUSTOM_TEST_FAIL(lineNumber,"(Token[0])Expected %d was %d", ((IntegerToken*)token1)->value, ((IntegerToken*)actualOper->token[0])->value);
  }
  else if(token1->type == TOKEN_FLOAT_TYPE){
    if(((FloatToken*)token1)->value != ((FloatToken*)actualOper->token[0])->value)
      CUSTOM_TEST_FAIL(lineNumber,"(Token[0])Expected %f was %f", ((FloatToken*)token1)->value, ((FloatToken*)actualOper->token[0])->value);    
  }
  else if(token1->type == TOKEN_OPERATOR_TYPE){
    if(strcmp(((OperatorToken*)token1)->symbol, ((OperatorToken*)actualOper->token[0])->symbol))
      CUSTOM_TEST_FAIL(lineNumber, "(Token[0])Expected symbol was '%s' but actual was '%s'",  \
      ((OperatorToken*)token1)->symbol, ((OperatorToken*)actualOper->token[0])->symbol);
    if(((OperatorToken*)token1)->arity != ((OperatorToken*)actualOper->token[0])->arity)
      CUSTOM_TEST_FAIL(lineNumber, "(Token[0])Expected arity was %d but actual was %d",       \
      ((OperatorToken*)token1)->arity, ((OperatorToken*)actualOper->token[0])->arity);
  }
  else{
    CUSTOM_TEST_FAIL(lineNumber,"Token[0] undefined!");
  }
//TEST for Token[1]
  if(token2->type == TOKEN_INTEGER_TYPE){
    if(((IntegerToken*)token2)->value != ((IntegerToken*)actualOper->token[1])->value)
      CUSTOM_TEST_FAIL(lineNumber,"(Token[1])Expected %d was %d", ((IntegerToken*)token2)->value, ((IntegerToken*)actualOper->token[1])->value);
  }
  else if(token2->type == TOKEN_FLOAT_TYPE){
    if(((FloatToken*)token2)->value != ((FloatToken*)actualOper->token[1])->value)
      CUSTOM_TEST_FAIL(lineNumber,"(Token[1])Expected %f was %f", ((FloatToken*)token2)->value, ((FloatToken*)actualOper->token[1])->value);    
  }
  else if(token2->type == TOKEN_OPERATOR_TYPE){
    if(strcmp(((OperatorToken*)token2)->symbol, ((OperatorToken*)actualOper->token[1])->symbol))
      CUSTOM_TEST_FAIL(lineNumber, "(Token[1])Expected symbol was '%s' but actual was '%s'",  \
      ((OperatorToken*)token2)->symbol, ((OperatorToken*)actualOper->token[1])->symbol);
    if(((OperatorToken*)token2)->arity != ((OperatorToken*)actualOper->token[1])->arity)
      CUSTOM_TEST_FAIL(lineNumber, "(Token[1])Expected arity was %d but actual was %d",       \
      ((OperatorToken*)token2)->arity, ((OperatorToken*)actualOper->token[1])->arity);
  }
  else{
    CUSTOM_TEST_FAIL(lineNumber,"Token[1] undefined!");
  }
}

void customTestAssertOperator(Token* expectedOper, OperatorToken *actualOper, int lineNumber){
  if(actualOper->type != TOKEN_OPERATOR_TYPE)
    CUSTOM_TEST_FAIL(lineNumber,"Expected OperatorToken!");
  if(strcmp(((OperatorToken*)expectedOper)->symbol, actualOper->symbol) != 0)
    CUSTOM_TEST_FAIL(lineNumber,"Expected symbol was '%s' but actual was '%s'", ((OperatorToken*)expectedOper)->symbol, actualOper->symbol);
  if(((OperatorToken*)expectedOper)->arity != actualOper->arity)
    CUSTOM_TEST_FAIL(lineNumber,"Expected arity was %d but actual %d", ((OperatorToken*)expectedOper)->arity, actualOper->arity);
  
}