#ifndef customAssertion_H
#define customAssertion_H

#include "unity.h"
#include "Token.h"

               // v----------->-------------->-----------v
// #define PRINT_MSG(x)      printf("%s, I am in line %d\n",x, __LINE__)


#define TEST_ASSERT_EQUAL_OPERATOR(symbol, token1, token2, oper);\
           customTestAssertOperator(symbol, token1, token2, oper);
 
void customTestAssertOperator(char *symbol, Token* token1, Token* token2, OperatorToken *oper);

// ... can represent any arguement.
// #define CUSTOM_TEST_FAIL(lineNo, msg, ...){                                                       \
                // char buffer[256];                                                                 \
                // sprintf(buffer, msg, ## __VA_ARGS__);                                             \
                // UNITY_TEST_FAIL(lineNo,buffer);                                                   \
// }
// #define _TEST_ASSERT_EQUAL(expected,actual)                                                       \
          // if(expected != actual) {                                                                \
                // CUSTOM_TEST_FAIL(__LINE__,"I expect %d, but I see %d.", expected, actual)           \
          // }
          
// #define TEST_ASSERT_COORDINATE(within,x,y,z,actual)                                               \
          // customTestAssertCoordinate(within,x,y,z,actual,__LINE__)
          
// void customTestAssertCoordinate(float within,
                               // float x,
                               // float y,
                               // float z,
                               // Coordinate actual,
                               // int lineNo);

#endif // customAssertion_H
