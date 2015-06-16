#include "unity.h"
#include "parser.h"
#include "mock_Token.h"
#include "mock_getToken.h"
#include <stdlib.h>
#include <stdio.h>

void setUp(void){}

void tearDown(void){}

void test_parser(void){
  OperatorToken* testOprToken = malloc(sizeof(OperatorToken*) + 2*(sizeof(Token*)));
  OperatorToken* lastOprToken = malloc(sizeof(OperatorToken*) + sizeof(Token*));
  IntegerToken* testIntToken = malloc(sizeof(IntegerToken*));
  IntegerToken* lastIntToken = malloc(sizeof(IntegerToken*));
//******************************************** FOR MOCKING
  testIntToken->type = TOKEN_INTEGER_TYPE;
  testIntToken->value = 2;
//******************************************** FOR MOCKING
  testOprToken->type = TOKEN_OPERATOR_TYPE;
  testOprToken->symbol = "+";
  testOprToken->arity = INFIX;
  testOprToken->bindingPower = ADD;
  printf("test BindingPower: %d\n", testOprToken->bindingPower);
//******************************************** FOR MOCKING
  lastIntToken->type = TOKEN_INTEGER_TYPE;
  lastIntToken->value = 3; 
//******************************************** FOR MOCKING
  lastOprToken->type = TOKEN_OPERATOR_TYPE;
  lastOprToken->symbol = "$";
  lastOprToken->arity = INFIX;
  lastOprToken->bindingPower = EOT; 

  getToken_ExpectAndReturn((Token*)testIntToken);
  getToken_ExpectAndReturn((Token*)testOprToken);
  getToken_ExpectAndReturn((Token*)lastIntToken);
  getToken_ExpectAndReturn((Token*)lastOprToken);
 
  Token* testToken = malloc(sizeof(Token*));
  testToken = parser(0);
//********************************************* START TEST
  TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE,((OperatorToken*)testToken)->type);
  TEST_ASSERT_EQUAL(TOKEN_INTEGER_TYPE,((OperatorToken*)testToken)->token[0]->type);
  TEST_ASSERT_EQUAL(TOKEN_INTEGER_TYPE,((OperatorToken*)testToken)->token[1]->type);
  
  TEST_ASSERT_EQUAL('+', *((OperatorToken*)testToken)->symbol);
  TEST_ASSERT_EQUAL(2,((IntegerToken*)((OperatorToken*)testToken)->token[0])->value);
  TEST_ASSERT_EQUAL(3,((IntegerToken*)((OperatorToken*)testToken)->token[1])->value);
}
