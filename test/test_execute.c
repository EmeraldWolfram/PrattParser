#include "unity.h"
#include "execute.h"
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

void test_executeExpression_with_3_EOT_should_print_3(void){
  Token* table[] = {
    createIntegerToken(3),
    createOperatorToken("$",POSTFIX),
    NULL
  };
  initTokenizer(table);
 
  Token* testToken = malloc(sizeof(Token));
  testToken = parser(0);
  char* equation = testToken->execute(testToken);
  printf("%s",equation);
  TEST_ASSERT_EQUAL_STRING("3", equation);
}  

void test_executeSingle_with_NOT_3_EOT_should_print_NOT3(void){
  Token* table[] = {
    createOperatorToken("!",INFIX),
    createIntegerToken(3),
    createOperatorToken("$",POSTFIX),
    NULL
  };
  initTokenizer(table);
 
  Token* testToken = malloc(sizeof(Token));
  testToken = parser(0);
  char* equation = testToken->execute(testToken);
  printf("%s",equation);
  TEST_ASSERT_EQUAL_STRING("(!3)", equation);
}

void test_excecuteDouble_given_2_ADD_3_will_return_2ADD3_tree(void){
  Token* table[] = {
    createIntegerToken(2),
    createOperatorToken("+",INFIX),
    createIntegerToken(3),
    createOperatorToken("$",POSTFIX),
    NULL
  };
  
  initTokenizer(table);
 
  Token* testToken  = malloc(sizeof(Token));
  testToken         = parser(0);
  char* equation    = testToken->execute(testToken);
  printf("%s",equation);
  TEST_ASSERT_EQUAL_STRING("(2 + 3)", equation);
}

void test_execute_given_2_ADD_3_SUB_4_EOT_should_return_2ADD3_then_SUB4_tree(void){
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
  
  Token* testToken = malloc(sizeof(Token));
  testToken = parser(0);
  char* equation    = testToken->execute(testToken);
  printf("%s",equation);
  TEST_ASSERT_EQUAL_STRING("((2 + 3) - 4)", equation);
}

void test_execute_with_2_ADD_3_MUL_4_SUB_minus_5_DIV_6_ADD_7_EOT_should_return_Token_Tree_1_SUB_Token_Tree_2_then_ADD_7(void){
  Token* table[] = {
    createIntegerToken(2),
    createOperatorToken("++",INFIX),
    createOperatorToken("+",INFIX),
    createIntegerToken(3),
    createOperatorToken("*",INFIX),
    createIntegerToken(4),
    createOperatorToken("-",INFIX),
    createOperatorToken("-",INFIX),
    createIntegerToken(5),
    createOperatorToken("/",INFIX),
    createIntegerToken(6),
    createOperatorToken("+",INFIX),
    createIntegerToken(7),
    createOperatorToken("$",POSTFIX),
    NULL
  };
  initTokenizer(table);
 
  Token* testToken = malloc(sizeof(Token));
  testToken = parser(0);
  char* equation    = testToken->execute(testToken);
  printf("%s",equation);
  TEST_ASSERT_EQUAL_STRING("((((++2) + (3 * 4)) - ((-5) / 6)) + 7)", equation);
}



