#include "unity.h"
#include "checkTokens.h"
#include "Token.h"
#include <stdlib.h>
#include <stdio.h>


void setUp(void){}

void tearDown(void){}

void test_checkToken_given_ADD_and_MUL_should_return_MUL(void){
	OperatorToken* mulToken = malloc(sizeof(OperatorToken)+sizeof(Token*)*2);
	mulToken = createOperatorToken("*",INFIX);
  
  OperatorToken* addToken = malloc(sizeof(OperatorToken)+sizeof(Token*)*2);
  addToken = createOperatorToken("+",INFIX);
  
  Token* highToken = malloc(sizeof(OperatorToken*));
  highToken = checkTokens(mulToken,addToken);
  
  TEST_ASSERT_EQUAL(MUL, ((OperatorToken*)highToken)->bindingPower);
}
