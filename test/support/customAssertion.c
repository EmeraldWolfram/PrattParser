#include "customAssertion.h"
#include "unity.h"

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

void customTestAssertOperator(char *symbol, int value1, int value2, OperatorToken *oper){
 TEST_ASSERT_EQUAL_STRING(symbol,oper->symbol);
 TEST_ASSERT_EQUAL(value1,((IntegerToken*)(oper->token[0]))->value);
 TEST_ASSERT_EQUAL(value2,((IntegerToken*)(oper->token[1]))->value);
}