#include "unity.h"
#include "parser.h"
#include "Token.h"
#include "mock_getToken.h"
#include "customAssertion.h"
#include <stdlib.h>
#include <stdio.h>
OperatorToken *multiply, *add, *subtract, *divide;
char *multiplication = "*";
char *addition = "+";
char *subtraction = "-";
char *division = "/";
IntegerToken* value_3, * value_4;
void setUp(void){}

void tearDown(void){}

void bindingPowerStrongerThanPreviousToken(OperatorToken* testOprToken,IntegerToken* testIntToken){
  peepToken_ExpectAndReturn((Token*)testIntToken);
  getToken_ExpectAndReturn((Token*)testIntToken);
  peepToken_ExpectAndReturn((Token*)testOprToken);
  getToken_ExpectAndReturn((Token*)testOprToken);
}

void bindingPowerWeakerThanPreviousToken(OperatorToken* testOprToken,IntegerToken* testIntToken){
  peepToken_ExpectAndReturn((Token*)testIntToken);
  getToken_ExpectAndReturn((Token*)testIntToken);
  peepToken_ExpectAndReturn((Token*)testOprToken);
}
  
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
  IntegerToken* testIntToken    = (IntegerToken*)createIntegerToken(2);
  OperatorToken* testOprToken   = (OperatorToken*)createOperatorToken("+",INFIX);

  IntegerToken* lastIntToken    = (IntegerToken*)createIntegerToken(3);
  OperatorToken* lastOprToken   = (OperatorToken*)createOperatorToken("$",POSTFIX);

//MOCK peepToken and getToken
  bindingPowerStrongerThanPreviousToken(testOprToken, testIntToken);    //Entered first call of parser
  bindingPowerWeakerThanPreviousToken(lastOprToken, lastIntToken);      //Entered second call of parser
  peepToken_ExpectAndReturn((Token*)lastOprToken);
 
  Token* testToken = malloc(sizeof(Token*));
  testToken = parser(0);
//********************************************* START TEST ************************************************************* 
  TEST_ASSERT_NOT_NULL(testToken);
  TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE,testToken->type);
  
  TEST_ASSERT_EQUAL_OPERATOR(addition,createIntegerToken(2),createIntegerToken(3),(OperatorToken*)testToken);
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
  IntegerToken* testIntToken_1    = (IntegerToken*)createIntegerToken(2);
  OperatorToken* testOprToken_1   = (OperatorToken*)createOperatorToken("+",INFIX);
  
  IntegerToken* testIntToken_2    = (IntegerToken*)createIntegerToken(3);
  OperatorToken* testOprToken_2   = (OperatorToken*)createOperatorToken("*",INFIX);
  
  IntegerToken* lastIntToken      = (IntegerToken*)createIntegerToken(4);
  OperatorToken* lastOprToken     = (OperatorToken*)createOperatorToken("$",POSTFIX);

//MOCK peepToken and getToken
  bindingPowerStrongerThanPreviousToken(testOprToken_1, testIntToken_1);    //In the first parser
  bindingPowerStrongerThanPreviousToken(testOprToken_2, testIntToken_2);    //Entered the second call of parser
  bindingPowerWeakerThanPreviousToken(lastOprToken,lastIntToken);           //Entered the third call of parser

  peepToken_ExpectAndReturn((Token*)lastOprToken);  
  peepToken_ExpectAndReturn((Token*)lastOprToken);
  
  Token* testToken = malloc(sizeof(Token*));
  testToken = parser(0);
//********************************************* START TEST ************************************************************* 
  TEST_ASSERT_NOT_NULL(testToken);
  TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE,testToken->type);
  
  TEST_ASSERT_EQUAL_OPERATOR(addition, createIntegerToken(2), createOperatorToken("*",INFIX), (OperatorToken*)testToken);
  multiply = (OperatorToken*)((OperatorToken*)testToken)->token[1];
  TEST_ASSERT_EQUAL_OPERATOR(multiplication, createIntegerToken(3), createIntegerToken(4), multiply);  
}

/**
 *
 *  Obtain tokens of 2 , + , 3 , * , 4 , - , 5
 *
 *  The parser should linked up and form a token tree as follow
 *
 *        (-)
 *        / \
 *      (+) (5)
 *      / \
 *    (2) (*)
 *        / \
 *      (3) (4)
 *
 *  Note: Symbol "$" was used here to indicate the end of Token
 */

void test_parser_with_2_ADD_3_MUL_4_SUB_5_EOT(void){
  IntegerToken* testIntToken_2      = (IntegerToken*)createIntegerToken(2);
  OperatorToken* testOprToken_ADD   = (OperatorToken*)createOperatorToken("+",INFIX);
  
  IntegerToken* testIntToken_3      = (IntegerToken*)createIntegerToken(3);
  OperatorToken* testOprToken_MUL   = (OperatorToken*)createOperatorToken("*",INFIX);
  
  IntegerToken* testIntToken_4      = (IntegerToken*)createIntegerToken(4);
  OperatorToken* testOprToken_SUB   = (OperatorToken*)createOperatorToken("-",INFIX);
  
  IntegerToken* lastIntToken_5      = (IntegerToken*)createIntegerToken(5);
  OperatorToken* lastOprToken       = (OperatorToken*)createOperatorToken("$",POSTFIX);

//MOCK peepToken and getToken
  bindingPowerStrongerThanPreviousToken(testOprToken_ADD, testIntToken_2);  //In the first parser
  bindingPowerStrongerThanPreviousToken(testOprToken_MUL, testIntToken_3);  //Entered the second call of parser
  bindingPowerWeakerThanPreviousToken(testOprToken_SUB, testIntToken_4);    //Entered the third call of parser
  peepToken_ExpectAndReturn((Token*)testOprToken_SUB);
  peepToken_ExpectAndReturn((Token*)testOprToken_SUB);
  getToken_ExpectAndReturn((Token*)testOprToken_SUB);
  bindingPowerWeakerThanPreviousToken(lastOprToken,lastIntToken_5);         //Entered the fourth call of parser
  peepToken_ExpectAndReturn((Token*)lastOprToken);
 
  Token* testToken = malloc(sizeof(Token*));
  testToken = parser(0);
//********************************************* START TEST ************************************************************* 
  TEST_ASSERT_NOT_NULL(testToken);
  TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE,testToken->type);
  
  add = (OperatorToken*)((OperatorToken*)testToken)->token[0];
  multiply = (OperatorToken*)add->token[1];
  TEST_ASSERT_EQUAL_OPERATOR(subtraction, createOperatorToken("+",INFIX), createIntegerToken(5), (OperatorToken*)testToken);
  TEST_ASSERT_EQUAL_OPERATOR(addition, createIntegerToken(2), createOperatorToken("*",INFIX), add);
  TEST_ASSERT_EQUAL_OPERATOR(multiplication, createIntegerToken(3), createIntegerToken(4), multiply);  
}

/**
 *
 *  Obtain tokens of 2, +, 3, *, 4, -, 5, /, 6, +, 7
 *
 *  The parser should linked up and form a token tree as follow
 *
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

void test_parser_with_2_ADD_3_MUL_4_SUB_5_DIV_6_ADD_7_EOT(void){
  IntegerToken* testIntToken_2      = (IntegerToken*)createIntegerToken(2);
  OperatorToken* testOprToken_ADD   = (OperatorToken*)createOperatorToken("+",INFIX);
  
  IntegerToken* testIntToken_3      = (IntegerToken*)createIntegerToken(3);
  OperatorToken* testOprToken_MUL   = (OperatorToken*)createOperatorToken("*",INFIX);
  
  IntegerToken* testIntToken_4      = (IntegerToken*)createIntegerToken(4);
  OperatorToken* testOprToken_SUB   = (OperatorToken*)createOperatorToken("-",INFIX);
  
  IntegerToken* testIntToken_5      = (IntegerToken*)createIntegerToken(5);
  OperatorToken* testOprToken_DIV   = (OperatorToken*)createOperatorToken("/",INFIX);
  
  IntegerToken* testIntToken_6      = (IntegerToken*)createIntegerToken(6);
  OperatorToken* testOprToken_ADD2  = (OperatorToken*)createOperatorToken("+",INFIX);
  
  IntegerToken* testIntToken_7      = (IntegerToken*)createIntegerToken(7);
  OperatorToken* lastOprToken       = (OperatorToken*)createOperatorToken("$",POSTFIX);

//MOCK peepToken and getToken
  bindingPowerStrongerThanPreviousToken(testOprToken_ADD, testIntToken_2);    //In the first parser
  bindingPowerStrongerThanPreviousToken(testOprToken_MUL, testIntToken_3);    //Entered the second call of parser
  bindingPowerWeakerThanPreviousToken(testOprToken_SUB, testIntToken_4);      //Entered the third call of parser
  peepToken_ExpectAndReturn((Token*)testOprToken_SUB);
  peepToken_ExpectAndReturn((Token*)testOprToken_SUB);
  getToken_ExpectAndReturn((Token*)testOprToken_SUB);

  bindingPowerStrongerThanPreviousToken(testOprToken_DIV, testIntToken_5);    //Entered the fourth call of parser
  bindingPowerWeakerThanPreviousToken(testOprToken_ADD2, testIntToken_6);     //Entered the fifth call of parser
  peepToken_ExpectAndReturn((Token*)testOprToken_ADD2);
  peepToken_ExpectAndReturn((Token*)testOprToken_ADD2);

  getToken_ExpectAndReturn((Token*)testOprToken_ADD2);                        //Entered the sixth call of parser
  bindingPowerWeakerThanPreviousToken(lastOprToken,testIntToken_7);
  peepToken_ExpectAndReturn((Token*)lastOprToken);
 
  Token* testToken = malloc(sizeof(Token*));
  testToken = parser(0);
//********************************************* START TEST ************************************************************* 
  TEST_ASSERT_NOT_NULL(testToken);
  TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE,testToken->type);
  TEST_ASSERT_EQUAL_OPERATOR(addition, createOperatorToken("-",INFIX), createIntegerToken(7), (OperatorToken*)testToken);

  subtract  = (OperatorToken*)((OperatorToken*)testToken)->token[0];    
  TEST_ASSERT_EQUAL_OPERATOR(subtraction, createOperatorToken("+",INFIX), createOperatorToken("/",INFIX), subtract);

  divide    = (OperatorToken*)subtract->token[1];                         
  add       = (OperatorToken*)subtract->token[0];                     
  TEST_ASSERT_EQUAL_OPERATOR(division, createIntegerToken(5), createIntegerToken(6), divide);
  TEST_ASSERT_EQUAL_OPERATOR(addition, createIntegerToken(2), createOperatorToken("*", INFIX), add);

  multiply  = (OperatorToken*)add->token[1];                                  
  TEST_ASSERT_EQUAL_OPERATOR(multiplication, createIntegerToken(3), createIntegerToken(4), multiply);
  
}













