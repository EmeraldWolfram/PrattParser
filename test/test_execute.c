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

/**
 *  With this token tree
 *
 *  (3)
 *
 *  Equation was printed "3".
 */
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

/**
 *  With this token tree
 *
 *         (!)
 *        /
 *      (3)
 *
 *  Equation was printed "(!3)".
 */
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
/**
 *  With this token tree
 *
 *      (+)
 *     /  \
 *   (2)  (3)
 *
 *  Equation was printed "(2 + 3)".
 */
void test_excecuteDouble_given_2_ADD_3_will_print_2ADD3(void){
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

/**
 *  With this token tree
 *
 *         (-)
 *        /  \
 *      (+)  (4)
 *     /  \
 *   (2)  (3)
 *
 *  Equation was printed "((2 + 3) - 4)".
 */
void test_execute_given_2_ADD_3_SUB_4_EOT_should_print_2ADD3_then_SUB4_tree(void){
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


/**
 *  With this token tree
 *
 *                    (+)
 *                  /   \ 
 *                (-)   (7)
 *              /    \
 *            /       \
 *          /          \
 *        (+)          (\)
 *       /  \         /  \
 *    (++)  (*)     (-)  (6)
 *    /     / \     /
 *  (2)   (3) (4) (5)
 *
 *  Equation was printed "((((++2) + (3 * 4)) - ((-5) / 6)) + 7)".
 */
void test_execute_with_2_INCR_ADD_3_MUL_4_SUB_minus_5_DIV_6_ADD_7_EOT_should_print_result(void){
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

/**
 *  With this token tree
 *
 *         ([)
 *        /
 *      (+)
 *     /  \
 *   (2)  (3)
 *
 *  Equation was printed "[(2 + 3)]".
 */
void test_execute_with_OPEN_2_ADD_3_CLOSE_EOT_should_print_OPEN_2_ADD_3_CLOSE(void){
  Token* table[] = {
    createOperatorToken("[",INFIX),
    createIntegerToken(2),
    createOperatorToken("+",INFIX),
    createIntegerToken(3),
    createOperatorToken("]",INFIX),
    createOperatorToken("$",POSTFIX),
    NULL
  };
  initTokenizer(table);
  
  Token* testToken = malloc(sizeof(Token));
  testToken = parser(0);
  char* equation    = testToken->execute(testToken);
  printf("%s",equation);
  TEST_ASSERT_EQUAL_STRING("[(2 + 3)]", equation);  
}

/**
 *  With this token tree
 *
 *            (()
 *           /  \
 *         (2)  (*)
 *              / \
 *            (3) (4)
 *
 *  Equation was printed "(2 ((3 * 4)))".
 */
void test_execute_with_2_OPEN_3_MUL_4_CLOSE_EOT_should_print_result(void){
  Token* table[] = { 
    createIntegerToken(2),
    createOperatorToken("(",INFIX),
    createIntegerToken(3),
    createOperatorToken("*",INFIX),
    createIntegerToken(4),
    createOperatorToken(")",INFIX),
    createOperatorToken("$",POSTFIX),
    NULL
  };
  initTokenizer(table);

  Token* testToken = malloc(sizeof(Token));
  testToken = parser(0);
  char* equation    = testToken->execute(testToken);
  printf("%s",equation);
  TEST_ASSERT_EQUAL_STRING("(2 ((3 * 4)))", equation);  
}
