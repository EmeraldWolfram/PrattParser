#include "unity.h"
#include "runOperation.h"
#include "mock_AddToken.h"
#include "Token.h"
#include <stdlib.h>
#include <stdio.h>

void setUp(void){}

void tearDown(void){}

void test_runOperation_given_2_add_3_should_return_5(void){
  OperatorToken* operation = malloc(sizeof(OperatorToken) + sizeof(Token*)*2);
  IntegerToken* leftTk = malloc(sizeof(IntegerToken*));
  IntegerToken* rightTk = malloc(sizeof(IntegerToken*));  

  operation = createOperatorToken("+", INFIX);
  operation->token[0] = (Token*)leftTk;
  operation->token[1] = (Token*)rightTk;
  
  add2Tokens_ExpectAndReturn(2,"+",3,(Token*)operation);
  add2Tokens(2,"+",3);
  leftTk->value = 2;
  rightTk->value = 3;
//START TEST FROM HERE, ABOVE ARE SETTING FOR MOCKING
  int answer = runOperation(operation);
  
  TEST_ASSERT_EQUAL(5,answer);
}
