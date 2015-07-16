#ifndef customAssertion_H
#define customAssertion_H

#include "unity.h"
#include "Token.h"

// ... can represent any arguement.
#define CUSTOM_TEST_FAIL(lineNo, msg, ...)                                                        \
                {                                                                                 \
                char buffer[256];                                                                 \
                sprintf(buffer, msg, ## __VA_ARGS__);                                             \
                UNITY_TEST_FAIL(lineNo,buffer);                                                   \
                }

#define TEST_ASSERT_EQUAL_TOKEN_TREE(expectedOper, token1, token2, actualOper);                   \
           customTestAssertTokenTree(expectedOper, token1, token2, actualOper, __LINE__);
        
#define TEST_ASSERT_EQUAL_OPERATOR(expectedOper, actualOper);                                     \
           customTestAssertOperator(expectedOper, actualOper, __LINE__);

void customTestAssertTokenTree(Token* expectedOper, Token* token1, Token* token2, OperatorToken *actualOper, int lineNumber);
void customTestAssertOperator(Token* expectedOper, OperatorToken *actualOper, int lineNumber);


#endif // customAssertion_H
