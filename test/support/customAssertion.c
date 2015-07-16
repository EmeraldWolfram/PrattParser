#include "customAssertion.h"
#include "unity.h"
#include "Token.h"

// void customTestAssertCoordinate(float within,
                               // float x,
                               // float y,
                               // float z,
                               // Coordinate actual,
                               // int lineNo){
  // if(!((x - within < actual.x)&&(actual.x < x + within)))
    // CUSTOM_TEST_FAIL(lineNo, "Expected x = %f, but was %f", x, actual.x);
  // if(!((y - within < actual.y)&&(actual.y < y + within)))
    // CUSTOM_TEST_FAIL(lineNo, "Expected x = %f, but was %f", y, actual.y);
  // if(!((z - within < actual.z)&&(actual.z < z + within)))
    // CUSTOM_TEST_FAIL(lineNo, "Expected x = %f, but was %f", z, actual.z);
// }

void customTestAssertOperator(char *symbol, Token* token1, Token* token2, OperatorToken *actualOper, int lineNumber){
  if(actualOper->type != TOKEN_OPERATOR_TYPE)
    CUSTOM_TEST_FAIL(lineNumber,"Expected OperatorToken!");
  if(strcmp(symbol, actualOper->symbol) != 0)
    CUSTOM_TEST_FAIL(lineNumber,"Expected '%s' was '%s'", symbol, actualOper->symbol);
  
  if(token1->type == TOKEN_INTEGER_TYPE){
    if(((IntegerToken*)token1)->value != ((IntegerToken*)actualOper->token[0])->value)
      CUSTOM_TEST_FAIL(lineNumber,"Expected %d was %d", ((IntegerToken*)token1)->value, ((IntegerToken*)actualOper->token[0])->value);
  }
  else if(token1->type == TOKEN_FLOAT_TYPE){
    if(((FloatToken*)token1)->value != ((FloatToken*)actualOper->token[0])->value)
      CUSTOM_TEST_FAIL(lineNumber,"Expected %f was %f", ((FloatToken*)token1)->value, ((FloatToken*)actualOper->token[0])->value);    
  }
  else if(token1->type == TOKEN_OPERATOR_TYPE){
    if(strcmp(((OperatorToken*)token1)->symbol, ((OperatorToken*)actualOper->token[0])->symbol))
      CUSTOM_TEST_FAIL(lineNumber, "Expected '%s' was '%s'", ((OperatorToken*)token1)->symbol, ((OperatorToken*)actualOper->token[0])->symbol);
    if(((OperatorToken*)token1)->arity != ((OperatorToken*)actualOper->token[0])->arity)
      CUSTOM_TEST_FAIL(lineNumber, "(Token[0])Expected %d was %d", ((OperatorToken*)token1)->arity, ((OperatorToken*)actualOper->token[0])->arity);
  }
  else{
    CUSTOM_TEST_FAIL(lineNumber,"token[0] undefined!");
  }

  if(token2->type == TOKEN_INTEGER_TYPE){
    if(((IntegerToken*)token2)->value != ((IntegerToken*)actualOper->token[1])->value)
      CUSTOM_TEST_FAIL(lineNumber,"Expected %d was %d", ((IntegerToken*)token2)->value, ((IntegerToken*)actualOper->token[1])->value);
  }
  else if(token2->type == TOKEN_FLOAT_TYPE){
    if(((FloatToken*)token2)->value != ((FloatToken*)actualOper->token[1])->value)
      CUSTOM_TEST_FAIL(lineNumber,"Expected %f was %f", ((FloatToken*)token2)->value, ((FloatToken*)actualOper->token[1])->value);    
  }
  else if(token2->type == TOKEN_OPERATOR_TYPE){
    if(strcmp(((OperatorToken*)token2)->symbol, ((OperatorToken*)actualOper->token[1])->symbol))
      CUSTOM_TEST_FAIL(lineNumber, "Expected '%s' was '%s'", ((OperatorToken*)token2)->symbol, ((OperatorToken*)actualOper->token[1])->symbol);
    if(((OperatorToken*)token2)->arity != ((OperatorToken*)actualOper->token[1])->arity)
      CUSTOM_TEST_FAIL(lineNumber, "(Token[1])Expected %d was %d", ((OperatorToken*)token2)->arity, ((OperatorToken*)actualOper->token[1])->arity);
  }
  else{
    CUSTOM_TEST_FAIL(lineNumber,"token[1] undefined!");
  }

}