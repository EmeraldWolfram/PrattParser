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

#define bindingPowerStrongerThanPreviousToken(testOprToken, testIntToken)     \
          getToken_ExpectAndReturn((Token*)testIntToken);                     \
          peepToken_ExpectAndReturn((Token*)testOprToken);                    \
          getToken_ExpectAndReturn((Token*)testOprToken);


#define bindingPowerWeakerThanPreviousToken(testOprToken, testIntToken)       \
          getToken_ExpectAndReturn((Token*)testIntToken);                     \
          peepToken_ExpectAndReturn((Token*)testOprToken);                    \

  
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

/**
 *
 *  Obtain tokens of - , 3 , * , - , 4
 *
 *  The parser should linked up and form a token tree as follow
 *
 *      (+)
 *      / \
 *    (-) (-)
 *    /   /
 *  (3) (4)
 *
 *  Note: Symbol "$" was used here to indicate the end of Token
 */
void test_parser_with_minus_3_MUL_minus_4_EOT(void){
  
  OperatorToken* preOprToken0   = (OperatorToken*)createOperatorToken("-",PREFIX);
  IntegerToken* testIntToken    = (IntegerToken*)createIntegerToken(3);
  OperatorToken* testOprToken   = (OperatorToken*)createOperatorToken("+",INFIX);

  
  OperatorToken* preOprToken1   = (OperatorToken*)createOperatorToken("-",PREFIX);
  IntegerToken* lastIntToken    = (IntegerToken*)createIntegerToken(4);
  OperatorToken* lastOprToken   = (OperatorToken*)createOperatorToken("$",POSTFIX);

//MOCK peepToken and getToken
  getToken_ExpectAndReturn((Token*)preOprToken0);
  getToken_ExpectAndReturn((Token*)testIntToken);
  peepToken_ExpectAndReturn((Token*)testOprToken);
  peepToken_ExpectAndReturn((Token*)testOprToken);
  getToken_ExpectAndReturn((Token*)testOprToken);
  
  getToken_ExpectAndReturn((Token*)preOprToken1);
  getToken_ExpectAndReturn((Token*)lastIntToken);
  peepToken_ExpectAndReturn((Token*)lastOprToken);
  peepToken_ExpectAndReturn((Token*)lastOprToken);
  peepToken_ExpectAndReturn((Token*)lastOprToken);
 
  Token* testToken = malloc(sizeof(Token*));
  testToken = parser(0);
//********************************************* START TEST ************************************************************* 
  TEST_ASSERT_NOT_NULL(testToken);
  TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE,testToken->type);
  
  TEST_ASSERT_EQUAL_OPERATOR(addition, createOperatorToken("-",PREFIX), createOperatorToken("-",PREFIX), (OperatorToken*)testToken);
  subtract = (OperatorToken*)((OperatorToken*)testToken)->token[0]; //Left Token
  TEST_ASSERT_EQUAL(3, ((IntegerToken*)subtract->token[0])->value);
  subtract = (OperatorToken*)((OperatorToken*)testToken)->token[1]; //Right Token
  TEST_ASSERT_EQUAL(4, ((IntegerToken*)subtract->token[0])->value);
}

/**
 *
 *  Obtain tokens of 2, +, 3, *, 4, -, 9, /, -, 9, +, 7
 *
 *  The parser should linked up and form a token tree as follow
 *
 *            (+)
 *            / \
 *          (-) (7)
 *         /   \
 *      (+)     (/)
 *      / \     / \
 *    (2) (*) (9) (-)
 *        / \     /
 *      (3) (4) (9)
 *
 *  Note: Symbol "$" was used here to indicate the end of Token
 */

void test_parser_with_2_ADD_3_MUL_4_SUB_9_DIV_minus_9_ADD_7_EOT(void){
  printf("\nFROM HERE\n");
  IntegerToken* testIntToken_2      = (IntegerToken*)createIntegerToken(2);
  OperatorToken* testOprToken_ADD   = (OperatorToken*)createOperatorToken("+",INFIX);
  
  IntegerToken* testIntToken_3      = (IntegerToken*)createIntegerToken(3);
  OperatorToken* testOprToken_MUL   = (OperatorToken*)createOperatorToken("*",INFIX);
  
  IntegerToken* testIntToken_4      = (IntegerToken*)createIntegerToken(4);
  OperatorToken* testOprToken_SUB   = (OperatorToken*)createOperatorToken("-",INFIX);
  
  IntegerToken* testIntToken_9      = (IntegerToken*)createIntegerToken(9);
  OperatorToken* testOprToken_DIV   = (OperatorToken*)createOperatorToken("/",INFIX);
  
  OperatorToken* preOprToken        = (OperatorToken*)createOperatorToken("-",PREFIX);
  IntegerToken* testIntToken_n9      = (IntegerToken*)createIntegerToken(9);
  OperatorToken* testOprToken_ADD2  = (OperatorToken*)createOperatorToken("+",INFIX);
  
  IntegerToken* testIntToken_7      = (IntegerToken*)createIntegerToken(7);
  OperatorToken* lastOprToken       = (OperatorToken*)createOperatorToken("$",POSTFIX);

//MOCK peepToken and getToken
  bindingPowerStrongerThanPreviousToken(testOprToken_ADD, testIntToken_2);    //In parser(0), formed (2 + parser(20))
  bindingPowerStrongerThanPreviousToken(testOprToken_MUL, testIntToken_3);    //In parser(20), formed (3 * parser(30)) 
  bindingPowerWeakerThanPreviousToken(testOprToken_SUB, testIntToken_4);      //In parser(30)), RETURN to parser(20) and complete with (3 * 4)
  
  peepToken_ExpectAndReturn((Token*)testOprToken_SUB);                        //RETURN to parser(0) and complete (2 + (3 * 4))
  peepToken_ExpectAndReturn((Token*)testOprToken_SUB);                        //Check for END of TOKEN      FALSE and continue
  
  getToken_ExpectAndReturn((Token*)testOprToken_SUB);                         //In parser(0), formed ((2 + (3 * 4)) - parser(20))
  bindingPowerStrongerThanPreviousToken(testOprToken_DIV, testIntToken_9);    //In parser(20), formed (9 / parser(30))
  getToken_ExpectAndReturn((Token*)preOprToken);                              //In parser(30), formed (- parser(100))
  bindingPowerWeakerThanPreviousToken(testOprToken_ADD2, testIntToken_n9);    //In parser(100), RETURN to parser(30) and complete with (-9)
  peepToken_ExpectAndReturn((Token*)testOprToken_ADD2);                       //In parser(30), check if "+" could win "/"
  
  peepToken_ExpectAndReturn((Token*)testOprToken_ADD2);                       //RETURN to parser(20) and complete (9 / (-9))
  peepToken_ExpectAndReturn((Token*)testOprToken_ADD2);                       //RETURN to parser(0) and complete ((2 + (3 * 4)) - (9 / (-9)))

  getToken_ExpectAndReturn((Token*)testOprToken_ADD2);                        //In parser(0), formed [((2 + (3 * 4)) - (9 / (-9))) + parser(20)]
  bindingPowerWeakerThanPreviousToken(lastOprToken,testIntToken_7);           //In parser(20), formed [((2 + (3 * 4)) - (9 / (-9))) + 7]
  peepToken_ExpectAndReturn((Token*)lastOprToken);                            //Check for END of TOKEN      TRUE and exit
 
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
  TEST_ASSERT_EQUAL_OPERATOR(division, createIntegerToken(9), createOperatorToken("-",PREFIX), divide);
  TEST_ASSERT_EQUAL_OPERATOR(addition, createIntegerToken(2), createOperatorToken("*", INFIX), add);
  subtract  = (OperatorToken*)(divide->token[1]);
  TEST_ASSERT_EQUAL(9, ((IntegerToken*)subtract->token[0])->value);
  multiply  = (OperatorToken*)add->token[1];                                  
  TEST_ASSERT_EQUAL_OPERATOR(multiplication, createIntegerToken(3), createIntegerToken(4), multiply);
}










