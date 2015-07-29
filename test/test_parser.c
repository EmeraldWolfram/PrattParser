#include "unity.h"
#include "parser.h"
#include "Token.h"
#include "tokenExtend.h"
#include "mock_getToken.h"
#include "ErrorObject.h"
#include "customAssertion.h"
#include "CException.h"
#include <stdlib.h>
#include <stdio.h>

OperatorToken *multiply, *add, *subtract, *divide, *open, *increment;

#define bindingPowerStrongerThanPreviousToken(testOprToken, testIntToken)     \
          getToken_ExpectAndReturn((Token*)testIntToken);                     \
          peepToken_ExpectAndReturn((Token*)testOprToken);                    \
          getToken_ExpectAndReturn((Token*)testOprToken);


#define bindingPowerWeakerThanPreviousToken(testOprToken, testIntToken)       \
          getToken_ExpectAndReturn((Token*)testIntToken);                     \
          peepToken_ExpectAndReturn((Token*)testOprToken);                    

          
ErrorObject* err;
void setUp(void){}

void tearDown(void){}
  
  
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
  IntegerToken* testIntToken    = (IntegerToken*)createIntegerToken(2);
  OperatorToken* testOprToken   = (OperatorToken*)createOperatorToken("+",INFIX);

  IntegerToken* lastIntToken    = (IntegerToken*)createIntegerToken(3);
  OperatorToken* lastOprToken   = (OperatorToken*)createOperatorToken("$",POSTFIX);

//MOCK peepToken and getToken
  bindingPowerStrongerThanPreviousToken(testOprToken, testIntToken);    //In parser(0), formed (2 + parser(20))
  bindingPowerWeakerThanPreviousToken(lastOprToken, lastIntToken);      //In parser(20), RETURN to parser(0) and completed (2 + 3)
  peepToken_ExpectAndReturn((Token*)lastOprToken);                      //In parser(0), peep '$' to check for EOT (End of Token) and RETURN Token Tree
 
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
  IntegerToken* testIntToken_1    = (IntegerToken*)createIntegerToken(2);
  OperatorToken* testOprToken_1   = (OperatorToken*)createOperatorToken("+",INFIX);
  
  IntegerToken* testIntToken_2    = (IntegerToken*)createIntegerToken(3);
  OperatorToken* testOprToken_2   = (OperatorToken*)createOperatorToken("-",INFIX);
  
  IntegerToken* lastIntToken      = (IntegerToken*)createIntegerToken(4);
  OperatorToken* lastOprToken     = (OperatorToken*)createOperatorToken("$",POSTFIX);

//MOCK peepToken and getToken
  bindingPowerStrongerThanPreviousToken(testOprToken_1, testIntToken_1);    //In parser(0), formed (2 + parser(20))
  bindingPowerWeakerThanPreviousToken(testOprToken_2, testIntToken_2);    	//In parser(20), RETURN to parser(0)  and completed (2 + 3)
  
  peepToken_ExpectAndReturn((Token*)testOprToken_2);                      //In parser(20), peep '-' to check for EOT and RETURN to parser(0)
  getToken_ExpectAndReturn((Token*)testOprToken_2);                      //In parser(0), peep '-' to check for EOT 
  bindingPowerWeakerThanPreviousToken(lastOprToken,lastIntToken);         //In parser(20), RETURN to parser(0) and completed [(2 + 3) - 4]
  peepToken_ExpectAndReturn((Token*)lastOprToken);
  
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
  IntegerToken* testIntToken_1    = (IntegerToken*)createIntegerToken(2);
  OperatorToken* testOprToken_1   = (OperatorToken*)createOperatorToken("+",INFIX);
  
  IntegerToken* testIntToken_2    = (IntegerToken*)createIntegerToken(3);
  OperatorToken* testOprToken_2   = (OperatorToken*)createOperatorToken("*",INFIX);
  
  IntegerToken* lastIntToken      = (IntegerToken*)createIntegerToken(4);
  OperatorToken* lastOprToken     = (OperatorToken*)createOperatorToken("$",POSTFIX);

//MOCK peepToken and getToken
  bindingPowerStrongerThanPreviousToken(testOprToken_1, testIntToken_1);    //In parser(0), formed (2 + parser(20))
  bindingPowerStrongerThanPreviousToken(testOprToken_2, testIntToken_2);    //In parser(20), formed (3 * parser(30))
  bindingPowerWeakerThanPreviousToken(lastOprToken,lastIntToken);           //In parser(30), RETURN to parser(20) and completed (2 + (3 * 4))
  
  peepToken_ExpectAndReturn((Token*)lastOprToken);                          //In parser(20), peep '$' to check for EOT and RETURN to parser(0)
  peepToken_ExpectAndReturn((Token*)lastOprToken);                          //In parser(0), peep '$' to check for EOT and RETURN Token Tree
  
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
  IntegerToken* testIntToken_2      = (IntegerToken*)createIntegerToken(2);
  OperatorToken* testOprToken_ADD   = (OperatorToken*)createOperatorToken("+",INFIX);
  
  IntegerToken* testIntToken_3      = (IntegerToken*)createIntegerToken(3);
  OperatorToken* testOprToken_MUL   = (OperatorToken*)createOperatorToken("*",INFIX);
  
  IntegerToken* testIntToken_4      = (IntegerToken*)createIntegerToken(4);
  OperatorToken* testOprToken_SUB   = (OperatorToken*)createOperatorToken("-",INFIX);
  
  IntegerToken* lastIntToken_5      = (IntegerToken*)createIntegerToken(5);
  OperatorToken* lastOprToken       = (OperatorToken*)createOperatorToken("$",POSTFIX);

//MOCK peepToken and getToken
  bindingPowerStrongerThanPreviousToken(testOprToken_ADD, testIntToken_2);  //In parser(0), formed (2 + parser(20))
  bindingPowerStrongerThanPreviousToken(testOprToken_MUL, testIntToken_3);  //In parser(20), formed (3 * parser(30))
  bindingPowerWeakerThanPreviousToken(testOprToken_SUB, testIntToken_4);    //In parser(30), RETURN to parser(20) and complete (2 + (3 * 4))
  
  peepToken_ExpectAndReturn((Token*)testOprToken_SUB);                      //In parser(20), peep '-' to check for EOT and RETURN to parser(0)
  peepToken_ExpectAndReturn((Token*)testOprToken_SUB);                      //In parser(0), peep '-' to check for EOT 
  getToken_ExpectAndReturn((Token*)testOprToken_SUB);                       //In parser(0), formed [(2 + (3 * 4)) - parser(20)]
  bindingPowerWeakerThanPreviousToken(lastOprToken,lastIntToken_5);         //In parser(20), RETURN to parser(0) and completed [(2 + (3 * 4)) - 5]
  
  peepToken_ExpectAndReturn((Token*)lastOprToken);                          //In parser(0), check for EOT and RETURN Token Tree
 
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
  bindingPowerStrongerThanPreviousToken(testOprToken_ADD, testIntToken_2);    //In parser(0), formed (2 + parser(20))
  bindingPowerStrongerThanPreviousToken(testOprToken_MUL, testIntToken_3);    //In parser(20), formed (3 * parser(30))
  bindingPowerWeakerThanPreviousToken(testOprToken_SUB, testIntToken_4);      //In parser(30), RETURN to parser(20) and complete (2 + (3 * 4))
  
  peepToken_ExpectAndReturn((Token*)testOprToken_SUB);                        //In parser(20), peep '-' to check for EOT and RETURN TO parser(0)
  peepToken_ExpectAndReturn((Token*)testOprToken_SUB);                        //In parser(0), peep '-' to check for EOT
  getToken_ExpectAndReturn((Token*)testOprToken_SUB);                         //In parser(0), formed [(2 + (3 * 4)) - parser(20)] 
  bindingPowerStrongerThanPreviousToken(testOprToken_DIV, testIntToken_5);    //In parser(20), formed (5 / parser(30))
  bindingPowerWeakerThanPreviousToken(testOprToken_ADD2, testIntToken_6);     //In parser(30), RETURN to parser(20) and complete [(2 + (3 * 4)) - (5 / 6)]
  
  peepToken_ExpectAndReturn((Token*)testOprToken_ADD2);                       //In parser(20), peep '+' to check for EOT and return to parser(0)
  peepToken_ExpectAndReturn((Token*)testOprToken_ADD2);                       //In parser(0), peep '+' to check for EOT 
  getToken_ExpectAndReturn((Token*)testOprToken_ADD2);                        //In parser(0), formed {[(2 + (3 * 4)) - (5 / 6)] + parser(20)}
  bindingPowerWeakerThanPreviousToken(lastOprToken,testIntToken_7);           //In parser(20), RETURN to parser(0) and complete {[(2 + (3 * 4)) - (5 / 6)] + 7}
  
  peepToken_ExpectAndReturn((Token*)lastOprToken);                            //In parser(0), peep '$' to check EOT and RETURN Token Tree
 
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

  OperatorToken* preOprToken0   = (OperatorToken*)createOperatorToken("-",PREFIX);
  IntegerToken* testIntToken    = (IntegerToken*)createIntegerToken(3);
  OperatorToken* lastOprToken   = (OperatorToken*)createOperatorToken("$",POSTFIX);

//MOCK peepToken and getToken
  getToken_ExpectAndReturn((Token*)preOprToken0);         //In parser(0), formed (- parser(100))
  getToken_ExpectAndReturn((Token*)testIntToken);         //In parser(100), get 3
  peepToken_ExpectAndReturn((Token*)lastOprToken);        //In parser(100), peep '$' to check binding power, RETURN to parser(0) and complete (-3)
  peepToken_ExpectAndReturn((Token*)lastOprToken);        //In parser(0), peep '$' to check binding power and RETURN Token Tree
 
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

  OperatorToken* preOprToken0   = (OperatorToken*)createOperatorToken("!",INFIX);
  IntegerToken* testIntToken    = (IntegerToken*)createIntegerToken(3);
  OperatorToken* lastOprToken   = (OperatorToken*)createOperatorToken("$",POSTFIX);

//MOCK peepToken and getToken
  getToken_ExpectAndReturn((Token*)preOprToken0);         //In parser(0), formed (- parser(100))
  getToken_ExpectAndReturn((Token*)testIntToken);         //In parser(100), get 3
  peepToken_ExpectAndReturn((Token*)lastOprToken);        //In parser(100), peep '$' to check binding power, RETURN to parser(0) and complete (-3)
  peepToken_ExpectAndReturn((Token*)lastOprToken);        //In parser(0), peep '$' to check binding power and RETURN Token Tree
 
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

  OperatorToken* preOprToken0   = (OperatorToken*)createOperatorToken("-",INFIX);
  OperatorToken* preOprToken1   = (OperatorToken*)createOperatorToken("-",INFIX);
  IntegerToken* testIntToken    = (IntegerToken*)createIntegerToken(3);
  OperatorToken* lastOprToken   = (OperatorToken*)createOperatorToken("$",POSTFIX);

//MOCK peepToken and getToken
  getToken_ExpectAndReturn((Token*)preOprToken0);                   //In parser(0), formed (- parser(100))
  getToken_ExpectAndReturn((Token*)preOprToken1);                  //In parser(100), formed (- - #parser(100))
  bindingPowerWeakerThanPreviousToken(lastOprToken,testIntToken);   //In #parser(100), formed (- - 3) and return to parser(100)
  peepToken_ExpectAndReturn((Token*)lastOprToken);                  //In parser(100), peep '$' to check binding power, RETURN to parser(0)
  peepToken_ExpectAndReturn((Token*)lastOprToken);                  //In parser(0), peep '$' to check binding power and RETURN Token Tree
 
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
  
  OperatorToken* preOprToken0   = (OperatorToken*)createOperatorToken("-",INFIX);
  IntegerToken* testIntToken    = (IntegerToken*)createIntegerToken(3);
  OperatorToken* testOprToken   = (OperatorToken*)createOperatorToken("+",INFIX);
  
  OperatorToken* preOprToken1   = (OperatorToken*)createOperatorToken("+",INFIX);
  IntegerToken* lastIntToken    = (IntegerToken*)createIntegerToken(4);
  OperatorToken* lastOprToken   = (OperatorToken*)createOperatorToken("$",POSTFIX);

//MOCK peepToken and getToken
  getToken_ExpectAndReturn((Token*)preOprToken0);         //In parser(0), formed (- parser(100))
  getToken_ExpectAndReturn((Token*)testIntToken);         //In parser(100), get 3
  peepToken_ExpectAndReturn((Token*)testOprToken);        //In parser(100), peep '+' to check binding power, RETURN to parser(0) and complete (-3)
  
  peepToken_ExpectAndReturn((Token*)testOprToken);        //In parser(0), peep '+' to check binding power
  getToken_ExpectAndReturn((Token*)testOprToken);         //In parser(0), formed ((-3) + parser(20)))
  
  getToken_ExpectAndReturn((Token*)preOprToken1);         //In parser(20), formed (+ parser(100))
  getToken_ExpectAndReturn((Token*)lastIntToken);         //In parser(100), get 4
  peepToken_ExpectAndReturn((Token*)lastOprToken);        //In parser(100), peep '$' to check binding power, RETURN to parser(20) and complete [(-3) + (+4)]
  peepToken_ExpectAndReturn((Token*)lastOprToken);        //In parser(20), peep '$' to check EOT and RETURN to parser(0)
  peepToken_ExpectAndReturn((Token*)lastOprToken);        //In parser(0), peep '$' to check EOT and RETURN Token Tree
 
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
  IntegerToken* testIntToken    = (IntegerToken*)createIntegerToken(2);
  OperatorToken* testOprToken   = (OperatorToken*)createOperatorToken("++", POSTFIX);
  OperatorToken* lastOprToken   = (OperatorToken*)createOperatorToken("$", POSTFIX);

//MOCK peepToken and getToken
  bindingPowerStrongerThanPreviousToken(testOprToken, testIntToken);    //In parser(0), formed (2 ++)
  peepToken_ExpectAndReturn((Token*)lastOprToken);                      //In parser(0), peep '$' to check for EOT and RETURN Token Tree

  Token* testToken = malloc(sizeof(Token*));
  testToken = parser(0);
//********************************************* START TEST ************************************************************* 
  TEST_ASSERT_NOT_NULL(testToken);
  
  TEST_ASSERT_EQUAL_STRING("++",((OperatorToken*)testToken)->symbol);
  TEST_ASSERT_EQUAL(2,((IntegerToken*)((OperatorToken*)testToken)->token[0])->value);
}

/**
 *
 *  Obtain tokens of 2 , --
 *
 *  This test check whether parser() function can differentiate INFIX and POSTFIX
 *  The parser should linked up and form a token tree as follow
 *
 *      (--)
 *      /
 *    (2)
 *
 *  Note: Symbol "$" was used here to indicate the end of Token
 */
void test_parser_with_2_INCREMENT_EOT_should_change_INFIX_to_POSTFIX(void){
  IntegerToken* testIntToken    = (IntegerToken*)createIntegerToken(2);
  OperatorToken* testOprToken   = (OperatorToken*)createOperatorToken("--", INFIX);
  OperatorToken* lastOprToken   = (OperatorToken*)createOperatorToken("$", POSTFIX);

//MOCK peepToken and getToken
  bindingPowerStrongerThanPreviousToken(testOprToken, testIntToken);    //In parser(0), formed (2 ++)
  peepToken_ExpectAndReturn((Token*)lastOprToken);                      //In parser(0), peep '$' to check for EOT and RETURN Token Tree

  Token* testToken = malloc(sizeof(Token*));
  testToken = parser(0);
//********************************************* START TEST ************************************************************* 
  TEST_ASSERT_NOT_NULL(testToken);
  
  TEST_ASSERT_EQUAL_OPERATOR(createOperatorToken("--",POSTFIX),((OperatorToken*)testToken));
  TEST_ASSERT_EQUAL(2,((IntegerToken*)((OperatorToken*)testToken)->token[0])->value);
}

/**
 *
 *  Obtain tokens of 2 , ++ , ++
 *
 *  This test check whether parser() function can link two consecutive POSTFIX
 *  The parser should linked up and form a token tree as follow
 *        
 *        (++)
 *        /
 *      (++)
 *      /
 *    (2)
 *
 *  Note: Symbol "$" was used here to indicate the end of Token
 */
void test_parser_with_2_INCREMENT_INCREMENT_EOT_should_return_2_inc_inc(void){
  IntegerToken* testIntToken     = (IntegerToken*)createIntegerToken(2);
  OperatorToken* testOprToken0   = (OperatorToken*)createOperatorToken("++", INFIX);
  OperatorToken* testOprToken1   = (OperatorToken*)createOperatorToken("++", INFIX);
  OperatorToken* lastOprToken    = (OperatorToken*)createOperatorToken("$", POSTFIX);

//MOCK peepToken and getToken
  bindingPowerStrongerThanPreviousToken(testOprToken0, testIntToken);    //In parser(0), formed (2 ++)
  peepToken_ExpectAndReturn((Token*)testOprToken1);                     
  getToken_ExpectAndReturn((Token*)testOprToken1);                      
  peepToken_ExpectAndReturn((Token*)lastOprToken);                      //In parser(0), peep '$' to check for EOT and RETURN Token Tree

  Token* testToken = malloc(sizeof(Token*));
  testToken = parser(0);
//********************************************* START TEST ************************************************************* 
  TEST_ASSERT_NOT_NULL(testToken);
  increment = (OperatorToken*)((OperatorToken*)testToken)->token[0];
  
  TEST_ASSERT_EQUAL_OPERATOR(createOperatorToken("++",POSTFIX),((OperatorToken*)testToken));
  TEST_ASSERT_EQUAL_OPERATOR(createOperatorToken("++",POSTFIX),increment);
  TEST_ASSERT_EQUAL(2,((IntegerToken*)increment->token[0])->value);
}



/**
 *
 *  Obtain tokens of ++ , 2 , * , 6 , -- , - , 8
 *  This test check parser() can differentiate between PREFIX, INFIX and POSTFIX as the '++' in this test
 *  is a PREFIX while the '--' should be a POSTFIX. At the same time, testing parser() function can link
 *  to INFIX correctly.
 *
 *  The parser should linked up and form a token tree as follow
 *
 *
 *             (-)
 *            /   \
 *          (*)   (8)
 *          / \
 *      (++)  (--)
 *      /     /
 *    (2)   (6)
 *
 *  Note: Symbol "$" was used here to indicate the end of Token
 */
void test_parser_with_INCR_2_MUL_6_DECR_SUB_8_EOT_should_return_INCR_2_then_MUL_with_Answer_of_DECR_6_then_SUB_8(void){
  OperatorToken* PreOprToken    = (OperatorToken*)createOperatorToken("++", INFIX);
  IntegerToken* testIntToken_2  = (IntegerToken*)createIntegerToken(2);
  OperatorToken* testOprToken_MUL   = (OperatorToken*)createOperatorToken("*", INFIX);

  IntegerToken* testIntToken_6  = (IntegerToken*)createIntegerToken(6);
  OperatorToken* PosOprToken    = (OperatorToken*)createOperatorToken("--", INFIX);
  OperatorToken* testOprToken_SUB   = (OperatorToken*)createOperatorToken("-", INFIX);
  
  IntegerToken* testIntToken_8  = (IntegerToken*)createIntegerToken(8);
  OperatorToken* lastOprToken   = (OperatorToken*)createOperatorToken("$", POSTFIX);

//MOCK peepToken and getToken
  getToken_ExpectAndReturn((Token*)PreOprToken);                        //In parser(0), formed (++ parser(100))
  getToken_ExpectAndReturn((Token*)testIntToken_2);                     //In parser(100), formed (++2)
  peepToken_ExpectAndReturn((Token*)testOprToken_MUL);                  //In parser(100), peep '*' to compare bindingPower and RETURN to parser(0)
  
  peepToken_ExpectAndReturn((Token*)testOprToken_MUL);                  //In parser(0), peep '*' to compare bindingPower
  getToken_ExpectAndReturn((Token*)testOprToken_MUL);                   //In parser(0), formed ((++2) * parser(30))
  bindingPowerStrongerThanPreviousToken(PosOprToken, testIntToken_6);   //In parser(30), formed (--6)
  peepToken_ExpectAndReturn((Token*)testOprToken_SUB);                  //In parser(30), RETURN to parser(30) and completed ((++2) * (--6))
  peepToken_ExpectAndReturn((Token*)testOprToken_SUB);                  //In parser(0), peep '-' to check for EOT
  getToken_ExpectAndReturn((Token*)testOprToken_SUB);                   //In parser(0), formed [((++2) * (--6)) - parser(20)]
  bindingPowerWeakerThanPreviousToken(lastOprToken,testIntToken_8);     //In parser(20), RETURN to parser(0) and completed [((++2) * (--6)) - 8]
  peepToken_ExpectAndReturn((Token*)lastOprToken);                      //In parser(0), peep '$' to check for EOT and RETURN Token Tree
 
  Token* testToken = malloc(sizeof(Token*));
  testToken = parser(0);
//********************************************* START TEST ************************************************************* 
  TEST_ASSERT_NOT_NULL(testToken);
  
  multiply = (OperatorToken*)((OperatorToken*)testToken)->token[0];
  TEST_ASSERT_EQUAL_TOKEN_TREE(createOperatorToken("-",INFIX), createOperatorToken("*",INFIX), createIntegerToken(8), (OperatorToken*)testToken);
  TEST_ASSERT_EQUAL_TOKEN_TREE(createOperatorToken("*",INFIX), createOperatorToken("++",PREFIX), createOperatorToken("--",POSTFIX), multiply);
  OperatorToken* token_INCR = ((OperatorToken*)multiply->token[0]);
  OperatorToken* token_DECR = ((OperatorToken*)multiply->token[1]);
  TEST_ASSERT_EQUAL(2, ((IntegerToken*)token_INCR->token[0])->value);
  TEST_ASSERT_EQUAL(6, ((IntegerToken*)token_DECR->token[0])->value);
}

//character '64' had been recognized as operator token.
//Check whether the parser will assign extendIntegerErrorOperator to OperatorToken '64'
void test_parser_with_2_64_3_EOT_to_throw_ERR_ILLEGAL_INTEGER(void){
  IntegerToken* testIntToken    = (IntegerToken*)createIntegerToken(2);
  OperatorToken* testOprToken   = (OperatorToken*)createOperatorToken("64",INFIX);

  IntegerToken* lastIntToken    = (IntegerToken*)createIntegerToken(3);
  OperatorToken* lastOprToken   = (OperatorToken*)createOperatorToken("$",POSTFIX);
  
  getToken_ExpectAndReturn((Token*)testIntToken);
  peepToken_ExpectAndReturn((Token*)testOprToken); 
          
  Try{
    Token* testToken = malloc(sizeof(Token*));  
    testToken = parser(0);
    TEST_FAIL_MESSAGE("Expected ERR_ILLEGAL_INTEGER but no error thrown!");
  }Catch(err){
    TEST_ASSERT_EQUAL(ERR_ILLEGAL_INTEGER, err->errorCode);
    TEST_ASSERT_EQUAL_STRING("Integer operator '64' is illegal!",  err->errorMsg);
    freeError(err);
  }
}

//character 'a' had been recognized as operator token.
//Check whether the parser will assign extendCharacterErrorOperator to OperatorToken 'a'
void test_parser_with_2_a_3_EOT_to_throw_ERR_ILLEGAL_CHARACTER(void){
  IntegerToken* testIntToken    = (IntegerToken*)createIntegerToken(2);
  OperatorToken* testOprToken   = (OperatorToken*)createOperatorToken("a",INFIX);

  IntegerToken* lastIntToken    = (IntegerToken*)createIntegerToken(3);
  OperatorToken* lastOprToken   = (OperatorToken*)createOperatorToken("$",POSTFIX);
  
  getToken_ExpectAndReturn((Token*)testIntToken);
  peepToken_ExpectAndReturn((Token*)testOprToken); 
          
  Try{
    Token* testToken = malloc(sizeof(Token*));  
    testToken = parser(0);
    TEST_FAIL_MESSAGE("Expected ERR_ILLEGAL_CHARACTER but no error thrown!");
  }Catch(err){
    TEST_ASSERT_EQUAL(ERR_ILLEGAL_CHARACTER, err->errorCode);
    TEST_ASSERT_EQUAL_STRING("Character operator 'a' is illegal!",  err->errorMsg);
    freeError(err);
  }
}

/**
 *  Obtain tokens of ( , 2 , + , 3 , ) , * , 4
 *                                   ^
 *                                   This token will be ignored
 *
 *  The parser should linked up and form a token tree as follow
 *  This test check if '(' can link in Nud function
 *
 *
 *            (*)
 *           /  \
 *         (()  (4)
 *        /
 *      (+)
 *     /  \
 *   (2)  (3)
 *
 *  Note: Symbol "$" was used here to indicate the end of Token
 */
void test_parser_with_OPEN_2_ADD_3_CLOSE_MUL_4_EOT_should_return_2_ADD_3_then_MUL_4(void){
  OperatorToken* testOprToken_OPEN  = (OperatorToken*)createOperatorToken("(",INFIX);
  IntegerToken* testIntToken_2      = (IntegerToken*)createIntegerToken(2);
  OperatorToken* testOprToken_ADD   = (OperatorToken*)createOperatorToken("+",INFIX);
  
  IntegerToken* testIntToken_3      = (IntegerToken*)createIntegerToken(3);
  OperatorToken* testOprToken_CLOSE = (OperatorToken*)createOperatorToken(")",INFIX);

  OperatorToken* testOprToken_MUL   = (OperatorToken*)createOperatorToken("*",INFIX);
 
  IntegerToken* lastIntToken_4      = (IntegerToken*)createIntegerToken(4);
  OperatorToken* lastOprToken       = (OperatorToken*)createOperatorToken("$",POSTFIX);

//MOCK peepToken and getToken
  getToken_ExpectAndReturn((Token*)testOprToken_OPEN);                      //In parser(0), formed (*parser(0))
  bindingPowerStrongerThanPreviousToken(testOprToken_ADD,testIntToken_2);   //In *parser(0), formed 2 + parser(20)
  bindingPowerWeakerThanPreviousToken(testOprToken_CLOSE,testIntToken_3);   //In parser(20), formed 2 + 3
  peepToken_ExpectAndReturn((Token*)testOprToken_CLOSE);                    //RETURN to *parser(0)
  getToken_ExpectAndReturn((Token*)testOprToken_CLOSE);                     //RETURN to parser(0) and completed (2 + 3)
  peepToken_ExpectAndReturn((Token*)testOprToken_MUL);                      //In parser(0), peep '*' for comparison
  getToken_ExpectAndReturn((Token*)testOprToken_MUL);                       //In parser(0), formed (2+3) * parser(30)
  bindingPowerWeakerThanPreviousToken(lastOprToken, lastIntToken_4);        //In parser(30), RETURN to parser(0) and completed (2 + 3) * 4
  peepToken_ExpectAndReturn((Token*)lastOprToken);                          //In parser(0), peep '$' to check for EOT and RETURN Token Tree
  
  Token* testToken = malloc(sizeof(Token*));
  testToken = parser(0);
//********************************************* START TEST ************************************************************* 
  TEST_ASSERT_NOT_NULL(testToken);
  
  TEST_ASSERT_EQUAL_TOKEN_TREE(createOperatorToken("*",INFIX), createOperatorToken("(", PREFIX), createIntegerToken(4), (OperatorToken*)testToken);
  open = (OperatorToken*)((OperatorToken*)testToken)->token[0];
  add  = (OperatorToken*)open->token[0];
  TEST_ASSERT_EQUAL_OPERATOR(createOperatorToken("+", INFIX), (OperatorToken*)open->token[0])
  TEST_ASSERT_EQUAL_TOKEN_TREE(createOperatorToken("+",INFIX), createIntegerToken(2), createIntegerToken(3), add);  
}

/**
 *  Obtain tokens of 2 , ( , 3 , * , 4 , )
 *                                       ^
 *                                       This token will be ignored
 *
 *  The parser should linked up and form a token tree as follow
 *  This test check if '(' can link in Led function
 *
 *
 *            (()
 *           /  \
 *         (2)  (*)
 *              / \
 *            (3) (4)
 *
 *  Note: Symbol "$" was used here to indicate the end of Token
 */
void test_parser_with_2_OPEN_3_MUL_4_CLOSE_EOT_should_return_3_MUL_4_then_OPEN_2(void){
  IntegerToken* testIntToken_2      = (IntegerToken*)createIntegerToken(2);
  OperatorToken* testOprToken_OPEN  = (OperatorToken*)createOperatorToken("(",INFIX);
  
  IntegerToken* testIntToken_3      = (IntegerToken*)createIntegerToken(3);
  OperatorToken* testOprToken_MUL   = (OperatorToken*)createOperatorToken("*",INFIX);
  
  IntegerToken* lastIntToken_4      = (IntegerToken*)createIntegerToken(4);
  OperatorToken* testOprToken_CLOSE = (OperatorToken*)createOperatorToken(")",INFIX);
  OperatorToken* lastOprToken       = (OperatorToken*)createOperatorToken("$",POSTFIX);

//MOCK peepToken and getToken
  bindingPowerStrongerThanPreviousToken(testOprToken_OPEN, testIntToken_2);  //In parser(0), formed 2 (*parser(0))
  bindingPowerStrongerThanPreviousToken(testOprToken_MUL, testIntToken_3);   //In *parser(0), formed 3 * parser(30)
  bindingPowerWeakerThanPreviousToken(testOprToken_CLOSE, lastIntToken_4);   //RETURN to *parser(0) and completed 3 * 4
  peepToken_ExpectAndReturn((Token*)testOprToken_CLOSE);                     //check for EOT/Close bracket
  getToken_ExpectAndReturn((Token*)testOprToken_CLOSE);                      //RETURN to parser(0) and completed 2 (3 * 4)
  peepToken_ExpectAndReturn((Token*)lastOprToken);                           //In parser(0), peep '$' to check for EOT and RETURN Token Tree
  
  Token* testToken = malloc(sizeof(Token*));
  testToken = parser(0);
// ********************************************* START TEST ************************************************************* 
  TEST_ASSERT_NOT_NULL(testToken);
  
  TEST_ASSERT_EQUAL_TOKEN_TREE(createOperatorToken("(",INFIX), createIntegerToken(2), createOperatorToken("*", INFIX), (OperatorToken*)testToken);
  multiply = (OperatorToken*)((OperatorToken*)testToken)->token[1];
  TEST_ASSERT_EQUAL_TOKEN_TREE(createOperatorToken("*",INFIX), createIntegerToken(3), createIntegerToken(4), multiply);  
}

/**
 *  Obtain tokens of ( , 2 , + , 3 , ) , 4
 *                                   ^
 *                                   This token will be ignored
 *
 *  The parser should linked up (2 + 3) and throw ERR_UNEXPECTED_EXPRESSION as it expect an operator after ")"
 *  Note: Symbol "$" was used here to indicate the end of Token
 */
void test_parser_with_OPEN_2_ADD_3_CLOSE_4_EOT_should_throw_ERR_UNEXPECTED_EXPRESSION(void){
  OperatorToken* testOprToken_OPEN  = (OperatorToken*)createOperatorToken("(",INFIX);
  IntegerToken* testIntToken_2      = (IntegerToken*)createIntegerToken(2);
  OperatorToken* testOprToken_ADD   = (OperatorToken*)createOperatorToken("+",INFIX);
  
  IntegerToken* testIntToken_3      = (IntegerToken*)createIntegerToken(3);
  OperatorToken* testOprToken_CLOSE = (OperatorToken*)createOperatorToken(")",INFIX);
 
  IntegerToken* lastIntToken_4      = (IntegerToken*)createIntegerToken(4);
  OperatorToken* lastOprToken       = (OperatorToken*)createOperatorToken("$",POSTFIX);

//MOCK peepToken and getToken
  getToken_ExpectAndReturn((Token*)testOprToken_OPEN);                      //In parser(0), formed (*parser(0))
  bindingPowerStrongerThanPreviousToken(testOprToken_ADD, testIntToken_2);  //In *parser(0), formed 2 + parser(20)
  bindingPowerWeakerThanPreviousToken(testOprToken_CLOSE,testIntToken_3);   //In parser(20), formed 2 + 3
  peepToken_ExpectAndReturn((Token*)testOprToken_CLOSE);                    //RETURN to *parser(0)
  getToken_ExpectAndReturn((Token*)testOprToken_CLOSE);                     //RETURN to parser(0) and completed (2 + 3)
  peepToken_ExpectAndReturn((Token*)lastIntToken_4);                        //In parser(0), peep '*' for comparison

  Try{
    Token* testToken = malloc(sizeof(Token*));
    testToken = parser(0);
    TEST_FAIL_MESSAGE("Expected ERR_UNEXPECTED_EXPRESSION but no error thrown!");
  }Catch(err){
    TEST_ASSERT_EQUAL(ERR_UNEXPECTED_EXPRESSION, err->errorCode);
    TEST_ASSERT_EQUAL_STRING("Expected operator token but obtained integer 4!",  err->errorMsg);
    freeError(err);
  }
}

/**
 *
 *  Obtain tokens of 2 , + , 3 , ) , * , 4
 *
 *  The parser will automatically apply a hidden open bracket at the first place
 *  to do the operation before the close bracket.
 *
 *  The parser should linked up and form a token tree as follow
 *  This test check if '+' can link to another OperatorToken ('*' in this case)
 *  
 *
 *      (*)
 *      / \
 *    (+) (4)
 *    / \
 *  (2) (3)
 *
 *  Note: Symbol "$" was used here to indicate the end of Token
 */
void test_parser_with_2_ADD_3_CLOSEBRACKET_MUL_4_EOT_should_return_3_CLOSEBRACKET_MUL_4_then_ADD_2(void){
  IntegerToken* testIntToken_1    = (IntegerToken*)createIntegerToken(2);
  OperatorToken* testOprToken_1   = (OperatorToken*)createOperatorToken("+",INFIX);
  
  IntegerToken* testIntToken_2    = (IntegerToken*)createIntegerToken(3);
  OperatorToken* testOprToken_2   = (OperatorToken*)createOperatorToken(")",NOFIX);
  OperatorToken* testOprToken_3   = (OperatorToken*)createOperatorToken("*",INFIX);
  
  IntegerToken* lastIntToken      = (IntegerToken*)createIntegerToken(4);
  OperatorToken* lastOprToken     = (OperatorToken*)createOperatorToken("$",POSTFIX);

//MOCK peepToken and getToken
  bindingPowerStrongerThanPreviousToken(testOprToken_1, testIntToken_1);        //In parser(0), formed 2 + parser(20)
  bindingPowerWeakerThanPreviousToken(testOprToken_2,testIntToken_2);           //In parser(20), RETURN to parser(0) and completed 2 + 3
  
  peepToken_ExpectAndReturn((Token*)testOprToken_2);                            //In parser(0), peep ')' to check for EOT
  getToken_ExpectAndReturn((Token*)testOprToken_2);                             //In parser(0), get ')' to move pointer to '*'
  
  peepToken_ExpectAndReturn((Token*)testOprToken_3);                            //In parser(0), peep '*' to check for EOT and RETURN to parser(0)
  getToken_ExpectAndReturn((Token*)testOprToken_3);                             //In parser(0), get '*' and formed (2 + 3) * parser(30) 
  
  bindingPowerWeakerThanPreviousToken(lastOprToken,lastIntToken);               //In parser(30), RETURN to parser(0) and completed (2 + 3) * 4
  peepToken_ExpectAndReturn((Token*)lastOprToken);                              //In parser(0), peep '$' to check for EOT and RETURN Token Tree
  
  Token* testToken = malloc(sizeof(Token*));
  testToken = parser(0);
//********************************************* START TEST ************************************************************* 
  TEST_ASSERT_NOT_NULL(testToken);
  
  TEST_ASSERT_EQUAL_TOKEN_TREE(createOperatorToken("*",INFIX), createOperatorToken("+", INFIX), createIntegerToken(4), (OperatorToken*)testToken);
  add = (OperatorToken*)((OperatorToken*)testToken)->token[0];
  TEST_ASSERT_EQUAL_TOKEN_TREE(createOperatorToken("+",INFIX), createIntegerToken(2), createIntegerToken(3), add);  
}

/**
 *
 *  Obtain tokens of ( , 2 , + , 3 , / , 4
 *
 *  The parser will automatically apply a hidden close bracket at the last place
 *  to complete the bracket.
 *
 *  The parser should linked up and form a token tree as follow
 *  This test check if '+' can link to another OperatorToken ('*' in this case)
 *
 *          (()
 *          /
 *        (+)
 *        / \
 *      (2) (/)
 *          / \
 *        (3) (4)
 *
 *  Note: Symbol "$" was used here to indicate the end of Token
 */
void test_parser_with_OPEN_2_ADD_3_DIV_4_EOT_should_return_OPEN_3_DIV_4_then_ADD_2(void){
  OperatorToken* preOprToken_OPEN = (OperatorToken*)createOperatorToken("(", INFIX);
  IntegerToken* testIntToken_2    = (IntegerToken*)createIntegerToken(2);
  OperatorToken* testOprToken_ADD = (OperatorToken*)createOperatorToken("+",INFIX);
  
  IntegerToken* testIntToken_3    = (IntegerToken*)createIntegerToken(3);
  OperatorToken* testOprToken_DIV = (OperatorToken*)createOperatorToken("/",INFIX);
  
  IntegerToken* lastIntToken      = (IntegerToken*)createIntegerToken(4);
  OperatorToken* lastOprToken     = (OperatorToken*)createOperatorToken("$",POSTFIX);

// MOCK peepToken and getToken
  getToken_ExpectAndReturn((Token*)preOprToken_OPEN);                           //In parser(0), formed  ( parser(1)
  bindingPowerStrongerThanPreviousToken(testOprToken_ADD, testIntToken_2);      //In parser(1), formed 2 + parser(50)
  bindingPowerStrongerThanPreviousToken(testOprToken_DIV, testIntToken_3);      //In parser(50), formed 3 / parser(60)
  bindingPowerWeakerThanPreviousToken(lastOprToken, lastIntToken);              //In parser(60), RETURN to parser(50) and completed 3 / 4
  peepToken_ExpectAndReturn((Token*)lastOprToken);                              //In parser(50), RETURN to parser(1) and completer 2 + (3 / 4)
  peepToken_ExpectAndReturn((Token*)lastOprToken);                              //In parser(1), RETURN to parser(0) and completed ( 2 + (3 / 4) ) HIDDEN CLOSE BRACKET added
  peepToken_ExpectAndReturn((Token*)lastOprToken);                              //In parser(1), peep '$' to check for EOT and RETURN Token Tree
  
  Token* testToken = malloc(sizeof(Token*));
  testToken = parser(0);

// ********************************************* START TEST ************************************************************* 
  TEST_ASSERT_NOT_NULL(testToken);
  
  TEST_ASSERT_EQUAL_OPERATOR(createOperatorToken("(",PREFIX), (OperatorToken*)testToken);
  add = (OperatorToken*)((OperatorToken*)testToken)->token[0];
  divide = (OperatorToken*)add->token[1];
  TEST_ASSERT_EQUAL_TOKEN_TREE(createOperatorToken("+",INFIX), createIntegerToken(2), createOperatorToken("/", INFIX), add);
  TEST_ASSERT_EQUAL_TOKEN_TREE(createOperatorToken("/",INFIX), createIntegerToken(3), createIntegerToken(4), divide);  
}