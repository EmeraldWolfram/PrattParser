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
  

void test_excecute_given_2_ADD_3_will_return_2ADD3_tree(void){
  Token* table[] = {
    createIntegerToken(2),
    createOperatorToken("+",INFIX),
    createIntegerToken(3),
    createOperatorToken("$",POSTFIX),
    NULL
  };
  
  initTokenizer(table);
 
  Token* testToken  = malloc(sizeof(Token*));
  testToken         = parser(0);
  char* equation    = testToken->execute(testToken);
  
  printf("%s",equation);
}
