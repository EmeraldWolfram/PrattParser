#include "unity.h"
#include "Token.h"
#include "AddToken.h"
#include <stdio.h>
#include <stdlib.h>

void setUp(void){}

void tearDown(void){}

void test_add2Tokens_given_2_and_3_should_return_the_token_tree(void){
  OperatorToken* addToken = malloc(sizeof(OperatorToken*)+sizeof(Token*)*2);
  addToken = (OperatorToken *)add2Tokens(2,"+",3);
  
  printf("leftToken type: %d",addToken->token[0]->type);
  
  TEST_ASSERT_EQUAL(TOKEN_INTEGER_TYPE,addToken->token[0]->type);
  TEST_ASSERT_EQUAL(TOKEN_INTEGER_TYPE,addToken->token[1]->type);
  TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE,addToken->type);
  
  TEST_ASSERT_EQUAL("+",addToken->symbol);
  TEST_ASSERT_EQUAL(INFIX,addToken->arity);
 
  TEST_ASSERT_EQUAL(2,((IntegerToken*)addToken->token[0])->value);
  TEST_ASSERT_EQUAL(3,((IntegerToken*)addToken->token[1])->value);
}
