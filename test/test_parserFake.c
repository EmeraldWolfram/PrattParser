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


/**
 *
 *  Obtain tokens of 2 , + , 3 , * , 4
 *
 *  The parser should linked up and form a token tree as follow
 *  This test check if '+' can link to another OperatorToken ('*' in this case)
 *
 *
 *      (+)
 *      / \
 *    (2) (*)
 *        / \
 *      (3) (4)
 *
 *  Note: Symbol "$" was used here to indicate the end of Token
 */
void test_parser_with_2_ADD_3_MUL_4_EOT_should_return_3_MUL_4_then_ADD_2(void){
  Token* table[] = {
    createIntegerToken(2),
    createOperatorToken("+",INFIX),
    createIntegerToken(3),
    createOperatorToken("*",INFIX),
    createIntegerToken(4),
    createOperatorToken("$",POSTFIX),
    NULL
  };
  initTokenizer(table);

  
  Token* testToken = malloc(sizeof(Token*));
  testToken = parser(0);
//********************************************* START TEST ************************************************************* 
  TEST_ASSERT_NOT_NULL(testToken);
  
  TEST_ASSERT_EQUAL_TOKEN_TREE(createOperatorToken("+",INFIX), createIntegerToken(2), createOperatorToken("*",INFIX), (OperatorToken*)testToken);
  multiply = (OperatorToken*)((OperatorToken*)testToken)->token[1];
  TEST_ASSERT_EQUAL_TOKEN_TREE(createOperatorToken("*",INFIX), createIntegerToken(3), createIntegerToken(4), multiply);  
}


/**
 *
 *  Obtain tokens of 2 , + , 3 , * , 4 , - , 5
 *
 *  This test check whether parser function can link down and link up Token Tree. 
 *  The parser should linked up and form a token tree as follow
 *
 *  Token Tree
 *    (+)               (-)
 *    / \      -->      / \
 *  (2) (*)    -->    (+) (5)
 *      / \    -->    / \
 *    (3) (4)       (2) (*)
 *                      / \
 *                    (3) (4)
 *
 *  Note: Symbol "$" was used here to indicate the end of Token
 */

void test_parser_with_2_ADD_3_MUL_4_SUB_5_EOT_should_return_Token_Tree_SUB_5(void){
  Token* table[] = {
    createIntegerToken(2),
    createOperatorToken("+",INFIX),
    createIntegerToken(3),
    createOperatorToken("*",INFIX),
    createIntegerToken(4),
    createOperatorToken("-",INFIX),
    createIntegerToken(5),
    createOperatorToken("$",POSTFIX),
    NULL
  };
  initTokenizer(table);
 
  Token* testToken = malloc(sizeof(Token*));
  testToken = parser(0);
//********************************************* START TEST ************************************************************* 
  TEST_ASSERT_NOT_NULL(testToken);
  
  add = (OperatorToken*)((OperatorToken*)testToken)->token[0];
  multiply = (OperatorToken*)add->token[1];
  TEST_ASSERT_EQUAL_TOKEN_TREE(createOperatorToken("-",INFIX), createOperatorToken("+",INFIX), createIntegerToken(5), (OperatorToken*)testToken);
  TEST_ASSERT_EQUAL_TOKEN_TREE(createOperatorToken("+",INFIX), createIntegerToken(2), createOperatorToken("*",INFIX), add);
  TEST_ASSERT_EQUAL_TOKEN_TREE(createOperatorToken("*",INFIX), createIntegerToken(3), createIntegerToken(4), multiply);  
}

/**
 *
 *  Obtain tokens of 2, +, 3, *, 4, -, 5, /, 6, +, 7
 *
 *  This test check whether it can link 2 token tree to a lower binding power operator without breaking.
 *  The parser should linked up and form a token tree as follow
 *
 *  After linked up this two Token Tree
 *  Token Tree 1        Token Tree 2 
 *      (+)               (/)
 *      / \               / \
 *    (2) (*)           (5) (6)
 *        / \
 *      (3) (4)
 *  
 *  Test whether parser() function can link two Token Tree above
 *            (+)
 *            / \
 *          (-) (7)
 *         /   \
 *      (+)     (/)
 *      / \     / \
 *    (2) (*) (5) (6)
 *        / \
 *      (3) (4)
 *
 *  Note: Symbol "$" was used here to indicate the end of Token
 */

void test_parser_with_2_ADD_3_MUL_4_SUB_5_DIV_6_ADD_7_EOT_should_return_Token_Tree_1_SUB_Token_Tree_2_then_ADD_7(void){
  Token* table[] = {
    createIntegerToken(2),
    createOperatorToken("+",INFIX),
    createIntegerToken(3),
    createOperatorToken("*",INFIX),
    createIntegerToken(4),
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
 
  Token* testToken = malloc(sizeof(Token*));
  testToken = parser(0);
//********************************************* START TEST ************************************************************* 
  TEST_ASSERT_NOT_NULL(testToken);
  TEST_ASSERT_EQUAL_TOKEN_TREE(createOperatorToken("+",INFIX), createOperatorToken("-",INFIX), createIntegerToken(7), (OperatorToken*)testToken);

  subtract  = (OperatorToken*)((OperatorToken*)testToken)->token[0];    
  TEST_ASSERT_EQUAL_TOKEN_TREE(createOperatorToken("-",INFIX), createOperatorToken("+",INFIX), createOperatorToken("/",INFIX), subtract);

  divide    = (OperatorToken*)subtract->token[1];                         
  add       = (OperatorToken*)subtract->token[0];                     
  TEST_ASSERT_EQUAL_TOKEN_TREE(createOperatorToken("/",INFIX), createIntegerToken(5), createIntegerToken(6), divide);
  TEST_ASSERT_EQUAL_TOKEN_TREE(createOperatorToken("+",INFIX), createIntegerToken(2), createOperatorToken("*", INFIX), add);

  multiply  = (OperatorToken*)add->token[1];                                  
  TEST_ASSERT_EQUAL_TOKEN_TREE(createOperatorToken("*",INFIX), createIntegerToken(3), createIntegerToken(4), multiply);
}

/**
 *
 *  Obtain tokens of - , 3 
 *  
 *  This test check whether parser() function can link prefix (negative in this case)
 *  The parser should linked up and form a token tree as follow
 *
 *    (-)
 *    /
 *  (3)
 *
 *  Note: Symbol "$" was used here to indicate the end of Token
 */
void test_parser_with_minus_3_EOT_should_return_SUB_3(void){
  Token* table[] = {
    createOperatorToken("-",PREFIX),
    createIntegerToken(3),
    createOperatorToken("$",POSTFIX),
    NULL
  };
  initTokenizer(table);
 
  Token* testToken = malloc(sizeof(Token*));
  testToken = parser(0);
//********************************************* START TEST ************************************************************* 
  TEST_ASSERT_NOT_NULL(testToken);
  
  TEST_ASSERT_EQUAL_STRING("-", ((OperatorToken*)testToken)->symbol);
  TEST_ASSERT_EQUAL(3, ((IntegerToken*)((OperatorToken*)testToken)->token[0])->value);
}

/**
 *
 *  Obtain tokens of ! , 3 
 *  
 *  This test check whether parser() function can differentiate PREFIX from INFIX
 *  The parser should linked up and form a token tree as follow
 *
 *    (!)
 *    /
 *  (3)
 *
 *  Note: Symbol "$" was used here to indicate the end of Token
 */
void test_parser_with_NOT_3_EOT_should_change_INFIX_NOT_to_PREFIX_NOT(void){
  Token* table[] = {
    createOperatorToken("!",INFIX),
    createIntegerToken(3),
    createOperatorToken("$",POSTFIX),
    NULL
  };
  initTokenizer(table);
 
  Token* testToken = malloc(sizeof(Token*));
  testToken = parser(0);
//********************************************* START TEST ************************************************************* 
  TEST_ASSERT_NOT_NULL(testToken);
  
  TEST_ASSERT_EQUAL_OPERATOR(createOperatorToken("!",PREFIX), (OperatorToken*)testToken);
  TEST_ASSERT_EQUAL(3, ((IntegerToken*)((OperatorToken*)testToken)->token[0])->value);
}

/**
 *
 *  Obtain tokens of - , - , 3 
 *  
 *  This test check whether parser() function can link two consecutive PREFIX operator
 *  The parser should linked up and form a token tree as follow
 *
 *      (-)
 *      /
 *    (-)
 *    /
 *  (3)
 *
 *  Note: Symbol "$" was used here to indicate the end of Token
 */
void test_parser_with_minus_minus_3_EOT_should_change_return_neg_neg_3(void){
  Token* table[] = {
    createOperatorToken("-",INFIX),
    createOperatorToken("-",INFIX),
    createIntegerToken(3),
    createOperatorToken("$",POSTFIX),
    NULL
  };
  initTokenizer(table);
 
  Token* testToken = malloc(sizeof(Token*));
  testToken = parser(0);
//********************************************* START TEST ************************************************************* 
  TEST_ASSERT_NOT_NULL(testToken);
  
  TEST_ASSERT_EQUAL_OPERATOR(createOperatorToken("-",PREFIX), (OperatorToken*)testToken);
  subtract = (OperatorToken*)((OperatorToken*)testToken)->token[0];
  TEST_ASSERT_EQUAL_OPERATOR(createOperatorToken("-",PREFIX), subtract);
  TEST_ASSERT_EQUAL(3, ((IntegerToken*)subtract->token[0])->value);
}

/**
 *
 *  Obtain tokens of - , 3 , * , - , 4
 *  
 *  This test check whether parser() function can link two PREFIX tree to a INFIX operator
 *  The parser should linked up and form a token tree as follow
 *
 *      (+)
 *      / \
 *    (-) (+)
 *    /   /
 *  (3) (4)
 *
 *  Note: Symbol "$" was used here to indicate the end of Token
 */
void test_parser_with_minus_3_MUL_minus_4_EOT_should_return_token_NEG_3_ADD_POS_4(void){
  Token* table[] = {
    createOperatorToken("-",INFIX),
    createIntegerToken(3),
    createOperatorToken("+",INFIX),
    createOperatorToken("+",INFIX),
    createIntegerToken(4),
    createOperatorToken("$",POSTFIX),
    NULL
  };
  initTokenizer(table);
  
  Token* testToken = malloc(sizeof(Token*));
  testToken = parser(0);
//********************************************* START TEST ************************************************************* 
  TEST_ASSERT_NOT_NULL(testToken);
  
  TEST_ASSERT_EQUAL_TOKEN_TREE(createOperatorToken("+",INFIX), createOperatorToken("-",PREFIX), createOperatorToken("+",PREFIX), (OperatorToken*)testToken);
  subtract  = (OperatorToken*)((OperatorToken*)testToken)->token[0]; //Left Token
  add       = (OperatorToken*)((OperatorToken*)testToken)->token[1]; //Right Token
  TEST_ASSERT_EQUAL(3, ((IntegerToken*)subtract->token[0])->value);
  TEST_ASSERT_EQUAL(4, ((IntegerToken*)add->token[0])->value);
}

/**
 *
 *  Obtain tokens of 2 , ++
 *
 *  This test check whether parser() function can handle postfix
 *  The parser should linked up and form a token tree as follow
 *
 *      (++)
 *      /
 *    (2)
 *
 *  Note: Symbol "$" was used here to indicate the end of Token
 */
void test_parser_with_2_INCREMENT_EOT_should_return_INCREMENT_2(void){
  Token* table[] = {
    createIntegerToken(2),
    createOperatorToken("++", POSTFIX),
    createOperatorToken("$", POSTFIX),
    NULL
  };
  initTokenizer(table);

  Token* testToken = malloc(sizeof(Token*));
  testToken = parser(0);
//********************************************* START TEST ************************************************************* 
  TEST_ASSERT_NOT_NULL(testToken);
  
  TEST_ASSERT_EQUAL_STRING("++",((OperatorToken*)testToken)->symbol);
  TEST_ASSERT_EQUAL(2,((IntegerToken*)((OperatorToken*)testToken)->token[0])->value);
}
