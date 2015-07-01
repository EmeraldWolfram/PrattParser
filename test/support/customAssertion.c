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

void customTestAssertOperator(char *symbol, Token* token1, Token* token2, OperatorToken *oper){
  TEST_ASSERT_EQUAL_STRING(symbol,oper->symbol);
  if(token1->type != TOKEN_OPERATOR_TYPE)
    TEST_ASSERT_EQUAL(((IntegerToken*)token1)->value, ((IntegerToken*)(oper->token[0]))->value);
  else
    TEST_ASSERT_EQUAL_STRING(((OperatorToken*)token1)->symbol,((OperatorToken*)oper->token[0])->symbol);
  
  if(token2->type != TOKEN_OPERATOR_TYPE)
    TEST_ASSERT_EQUAL(((IntegerToken*)token2)->value, ((IntegerToken*)(oper->token[1]))->value);
  else
    TEST_ASSERT_EQUAL_STRING(((OperatorToken*)token2)->symbol,((OperatorToken*)oper->token[1])->symbol);
}