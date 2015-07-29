#include "unity.h"
#include "parser.h"
#include "Token.h"
#include "tokenExtend.h"
#include "mock_getToken.h"
#include "Common.h"
#include "ErrorObject.h"
#include "customAssertion.h"
#include "CException.h"
#include <stdlib.h>
#include <stdio.h>

OperatorToken *multiply, *add, *subtract, *divide, *open, *increment;
ErrorObject *err;

#define bindingPowerStrongerThanPreviousToken(testOprToken, testIntToken)     \
          getToken_ExpectAndReturn((Token*)testIntToken);                     \
          peepToken_ExpectAndReturn((Token*)testOprToken);                    \
          getToken_ExpectAndReturn((Token*)testOprToken);


#define bindingPowerWeakerThanPreviousToken(testOprToken, testIntToken)       \
          getToken_ExpectAndReturn((Token*)testIntToken);                     \
          peepToken_ExpectAndReturn((Token*)testOprToken);                    

// Global variable(s) used by getToken() and peepToken()
Token **tokenTable = NULL;

Token *fake_getToken(int numOfCalls) {
  if(*tokenTable)
    return *tokenTable++;
  return *tokenTable;
}

Token *fake_peepToken(int numOfCalls) {
  return *tokenTable;
}

void initTokenizer(Token *table[]) {
  tokenTable = table;
}

void setUp(void){
  getToken_StubWithCallback(fake_getToken);
  peepToken_StubWithCallback(fake_peepToken);
}

void tearDown(void){
  tokenTable = NULL;
}
  
  
/**
 *
 *  Obtain tokens of 2 , + , 3
 *
 *  The parser should linked up and form a token tree as follow
 *  This test check whether '+' can link 2 and 3 together
 *
 *      (+)
 *      / \
 *    (2) (3)
 *
 *  Note: Symbol "$" was used here to indicate the end of Token
 */
void test_parser_with_2_ADD_3_EOT_should_return_2_ADD_3(void){
  Token* table[] = {
    createIntegerToken(2),
    createOperatorToken("+",INFIX),
    createIntegerToken(3),
    createOperatorToken("$",POSTFIX),
    NULL
  };
  
  initTokenizer(table);
 
  Token* testToken = malloc(sizeof(Token*));
  testToken = parser(0);
//********************************************* START TEST ************************************************************* 
  TEST_ASSERT_NOT_NULL(testToken);
  TEST_ASSERT_EQUAL_TOKEN_TREE(createOperatorToken("+",INFIX),createIntegerToken(2),createIntegerToken(3),(OperatorToken*)testToken);
}

/**
 *
 *  Obtain tokens of 2 , + , 3 , - , 4
 *
 *  The parser should linked up and form a token tree as follow
 *  This test check if '+' can link to another OperatorToken ('-' in this case)
 *
 *
 *     (-)
 *     / \
 *   (+) (4)
 *   / \
 * (2) (3)
 *
 *  Note: Symbol "$" was used here to indicate the end of Token
 */
void test_parser_with_2_ADD_3_SUB_4_EOT_should_return_3_MUL_4_then_ADD_2(void){
  Token* table[] = {
    createIntegerToken(2),
    createOperatorToken("+",INFIX),
    createIntegerToken(3),
    createOperatorToken("-",INFIX),
    createIntegerToken(4),
    createOperatorToken("$",POSTFIX),
    NULL
  };
  initTokenizer(table);
  
  Token* testToken = malloc(sizeof(Token*));
  testToken = parser(0);
//********************************************* START TEST ************************************************************* 
  TEST_ASSERT_NOT_NULL(testToken);
  
  TEST_ASSERT_EQUAL_TOKEN_TREE((Token*)createOperatorToken("-",INFIX), createOperatorToken("+",INFIX), createIntegerToken(4), (OperatorToken*)testToken);
  add = (OperatorToken*)((OperatorToken*)testToken)->token[0];
  TEST_ASSERT_EQUAL_TOKEN_TREE((Token*)createOperatorToken("+",INFIX), createIntegerToken(2), createIntegerToken(3), add);  
}
