#include "unity.h"
#include "parser.h"
#include "mock_Token.h"
#include "mock_getToken.h"
#include <stdlib.h>
#include <stdio.h>

void setUp(void){}

void tearDown(void){}

/**
 *
 *  Obtain tokens of 2 , + , 3
 *
 *  The parser should linked up and form a token tree as follow
 *
 *      (+)
 *      / \
 *    (2) (3)
 *
 *  Note: Symbol "$" was used here to indicate the end of Token
 */
void test_parser_with_2_ADD_3_EOT(void){
  OperatorToken* testOprToken = malloc(sizeof(OperatorToken) + 2*(sizeof(Token*)));
  OperatorToken* lastOprToken = malloc(sizeof(OperatorToken) + sizeof(Token*));
  IntegerToken* testIntToken = malloc(sizeof(IntegerToken));
  IntegerToken* lastIntToken = malloc(sizeof(IntegerToken));
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

  peepToken_ExpectAndReturn((Token*)testIntToken);
  getToken_ExpectAndReturn((Token*)testIntToken);
  peepToken_ExpectAndReturn((Token*)testOprToken);
  getToken_ExpectAndReturn((Token*)testOprToken);
  peepToken_ExpectAndReturn((Token*)lastIntToken);
  getToken_ExpectAndReturn((Token*)lastIntToken);
  peepToken_ExpectAndReturn((Token*)lastOprToken);  
  peepToken_ExpectAndReturn((Token*)lastOprToken);    
 
  Token* testToken = malloc(sizeof(Token*));
  testToken = parser(0);
//********************************************* START TEST  
  TEST_ASSERT_NOT_NULL(testToken);
  TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE,((IntegerToken*)testToken)->type);
  
  TEST_ASSERT_EQUAL('+', *((OperatorToken*)testToken)->symbol);
  TEST_ASSERT_EQUAL(2,((IntegerToken*)((OperatorToken*)testToken)->token[0])->value);
  TEST_ASSERT_EQUAL(3,((IntegerToken*)((OperatorToken*)testToken)->token[1])->value);
}

/**
 *
 *  Obtain tokens of 2 , + , 3 , * , 4
 *
 *  The parser should linked up and form a token tree as follow
 *
 *      (+)
 *      / \
 *    (2) (*)
 *        / \
 *      (3) (4)
 *
 *  Note: Symbol "$" was used here to indicate the end of Token
 */
void test_parser_with_2_ADD_3_MUL_4_EOT(void){
  OperatorToken* testOprToken_1 = malloc(sizeof(OperatorToken) + 2*(sizeof(Token*)));
  OperatorToken* testOprToken_2 = malloc(sizeof(OperatorToken) + 2*(sizeof(Token*)));
  OperatorToken* lastOprToken = malloc(sizeof(OperatorToken) + sizeof(Token*));
  IntegerToken* testIntToken_1 = malloc(sizeof(IntegerToken));
  IntegerToken* testIntToken_2 = malloc(sizeof(IntegerToken));
  IntegerToken* lastIntToken = malloc(sizeof(IntegerToken));
//******************************************** FOR MOCKING
  testIntToken_1->type = TOKEN_INTEGER_TYPE;
  testIntToken_1->value = 2;
//******************************************** FOR MOCKING
  testOprToken_1->type = TOKEN_OPERATOR_TYPE;
  testOprToken_1->symbol = "+";
  testOprToken_1->arity = INFIX;
  testOprToken_1->bindingPower = ADD;
  printf("test BindingPower: %d\n", testOprToken_1->bindingPower);
//******************************************** FOR MOCKING
  testIntToken_2->type = TOKEN_INTEGER_TYPE;
  testIntToken_2->value = 3;
//******************************************** FOR MOCKING
  testOprToken_2->type = TOKEN_OPERATOR_TYPE;
  testOprToken_2->symbol = "*";
  testOprToken_2->arity = INFIX;
  testOprToken_2->bindingPower = MUL;
  printf("*test BindingPower: %d\n", testOprToken_2->bindingPower);
//******************************************** FOR MOCKING
  lastIntToken->type = TOKEN_INTEGER_TYPE;
  lastIntToken->value = 4; 
//******************************************** FOR MOCKING
  lastOprToken->type = TOKEN_OPERATOR_TYPE;
  lastOprToken->symbol = "$";
  lastOprToken->arity = INFIX;
  lastOprToken->bindingPower = EOT; 

  peepToken_ExpectAndReturn((Token*)testIntToken_1);
  getToken_ExpectAndReturn((Token*)testIntToken_1);
  peepToken_ExpectAndReturn((Token*)testOprToken_1);
  getToken_ExpectAndReturn((Token*)testOprToken_1);
  peepToken_ExpectAndReturn((Token*)testIntToken_2);
  getToken_ExpectAndReturn((Token*)testIntToken_2);
  peepToken_ExpectAndReturn((Token*)testOprToken_2);
  getToken_ExpectAndReturn((Token*)testOprToken_2);
  peepToken_ExpectAndReturn((Token*)lastIntToken);
  getToken_ExpectAndReturn((Token*)lastIntToken);
  peepToken_ExpectAndReturn((Token*)lastOprToken);  
  peepToken_ExpectAndReturn((Token*)lastOprToken);    
  peepToken_ExpectAndReturn((Token*)lastOprToken);    
 
  Token* testToken = malloc(sizeof(Token*));
  testToken = parser(0);
//********************************************* START TEST  
  TEST_ASSERT_NOT_NULL(testToken);
  TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE,((IntegerToken*)testToken)->type);
  
  TEST_ASSERT_EQUAL('+', *((OperatorToken*)testToken)->symbol);
  TEST_ASSERT_EQUAL(2,((IntegerToken*)((OperatorToken*)testToken)->token[0])->value);
  TEST_ASSERT_EQUAL("*",((OperatorToken*)((OperatorToken*)testToken)->token[1])->symbol);
  TEST_ASSERT_EQUAL(3,((IntegerToken*)((OperatorToken*)((OperatorToken*)testToken)->token[1])->token[0])->value);
  TEST_ASSERT_EQUAL(4,((IntegerToken*)((OperatorToken*)((OperatorToken*)testToken)->token[1])->token[1])->value);
}