#include "unity.h"
#include <stdlib.h>
#include "Token.h"
#include "ErrorObject.h"
#include "tokenExtend.h"
#include "mock_parser.h"

OperatorAttributes operatorAttributesTable[] = {
  ['<'] = {30, INFIX, infixNud, infixLed, extendQuadrupleCharacterOperator},
  ['>'] = {30, INFIX, infixNud, infixLed, extendQuadrupleCharacterOperator},
  ['+'] = {50, PREFIX, infixNud, infixLed, extendTripleCharacterOperator},
  ['-'] = {50, PREFIX, infixNud, infixLed, extendTripleCharacterOperator},
  ['&'] = {10, PREFIX, infixNud, infixLed, extendTripleCharacterOperator},
  ['|'] = { 8, INFIX, infixNud, infixLed, extendTripleCharacterOperator},
  ['*'] = {60, PREFIX, infixNud, infixLed, extendDoubleCharacterOperator},
  ['/'] = {60, INFIX, infixNud, infixLed, extendDoubleCharacterOperator},
  ['%'] = {60, INFIX, infixNud, infixLed, extendDoubleCharacterOperator},
  ['^'] = { 9, INFIX, infixNud, infixLed, extendDoubleCharacterOperator},
  ['!'] = {70, INFIX, infixNud, infixLed, extendDoubleCharacterOperator},
  ['='] = { 5, INFIX, infixNud, infixLed, extendDoubleCharacterOperator},
  ['~'] = {70, PREFIX, infixNud, infixLed, extendSingleCharacterOperator},
  ['('] = {70, PREFIX, infixNud, infixLed, extendSingleCharacterOperator},
  ['['] = {70, PREFIX, infixNud, infixLed, extendSingleCharacterOperator}
};

void setUp(void){}

void tearDown(void){}

void test_extendSingleCharacterOperator_given_INVERT_and_Table_should_assign_70_PREFIX(void){
  OperatorToken* invert   = malloc(sizeof(OperatorToken) + sizeof(OperatorAttributes));
  invert->symbol          = "~";
  int thisSymbol          = (int)(*invert->symbol);
  OperatorAttributes oprAttr  = operatorAttributesTable[thisSymbol];
  
  invert = (OperatorToken*)oprAttr.extend((Token*)invert, &oprAttr);
  
  TEST_ASSERT_EQUAL(PREFIX, invert->arity);
  TEST_ASSERT_EQUAL(70, invert->bindingPower);
}

void test_extendDoubleCharacterOperator_given_DIV_and_Table_should_assign_60_INFIX(void){
  OperatorToken* div  = malloc(sizeof(OperatorToken) + sizeof(OperatorAttributes));
  div->symbol         = "/";
  int thisSymbol      = (int)(*div->symbol);
  OperatorAttributes oprAttr  = operatorAttributesTable[thisSymbol];
  
  div = (OperatorToken*)oprAttr.extend((Token*)div, &oprAttr);
  
  TEST_ASSERT_EQUAL(INFIX, div->arity);
  TEST_ASSERT_EQUAL(60, div->bindingPower);
}

void test_extendTripleCharacterOperator_given_INCR_and_Table_should_assign_70_PREFIX(void){
  OperatorToken* incr  = malloc(sizeof(OperatorToken) + sizeof(OperatorAttributes));
  incr->symbol         = "++";
  int thisSymbol      = (int)(*incr->symbol);
  OperatorAttributes oprAttr  = operatorAttributesTable[thisSymbol];
  
  incr = (OperatorToken*)oprAttr.extend((Token*)incr, &oprAttr);
  
  TEST_ASSERT_EQUAL(PREFIX, incr->arity);
  TEST_ASSERT_EQUAL(70, incr->bindingPower);
}

void test_extendQuadrupleCharacterOperator_given_ROTATE_and_Table_should_assign_70_INFIX(void){
  OperatorToken* rotate   = malloc(sizeof(OperatorToken) + sizeof(OperatorAttributes));
  rotate->symbol          = "<<";
  int thisSymbol          = (int)(*rotate->symbol);
  OperatorAttributes oprAttr  = operatorAttributesTable[thisSymbol];
  
  rotate = (OperatorToken*)oprAttr.extend((Token*)rotate, &oprAttr);
  
  TEST_ASSERT_EQUAL(INFIX, rotate->arity);
  TEST_ASSERT_EQUAL(40, rotate->bindingPower);
}

/*  createOperatorToken("*", INFIX) should create a pointer to OperatorToken
 *
 *                                     [OperatorToken]
 *  [OperatorToken ptr] -------------> [      *      ]
 *                                     [    INFIX    ]
 *
 *  When this token sub into infixNud,
 *  this test will show an error message to show that "*" is not a prefix operator.
 *
 */
void test_infixNud_given_Token_of_Prefix_symbol_MUL_should_show_error_msg(void){
  OperatorToken* subToken = malloc(sizeof(OperatorToken));
  OperatorToken* subToken1 = malloc(sizeof(OperatorToken));
  subToken = (OperatorToken*)createOperatorToken("*", INFIX);
  
  subToken1 =  (OperatorToken*)infixNud((Token*)subToken);
  TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE, subToken->type);
  TEST_ASSERT_EQUAL("*", subToken->symbol);
  TEST_ASSERT_EQUAL(INFIX, subToken->arity);
  TEST_ASSERT_EQUAL(MUL, subToken->bindingPower);
}

/*  createOperatorToken("s", INFIX) should create a pointer to OperatorToken
 *
 *                                     [OperatorToken]
 *  [OperatorToken ptr] -------------> [      s      ]
 *                                     [    INFIX    ]
 *
 *  This token is TOKEN_UNKNOWN_TYPE.
 *  When this token sub into infixLed,
 *  this test will show an error message to show that "s" is undefined operator.
 *
 */
void test_infixLed_given_Token_of_Infix_symbol_s_should_show_error_msg(void){
  OperatorToken* subToken = malloc(sizeof(OperatorToken));
  OperatorToken* subToken1 = malloc(sizeof(OperatorToken));
  subToken = (OperatorToken*)createOperatorToken("s", INFIX);
  
  subToken->type    = TOKEN_UNKNOWN_TYPE;
  
  subToken1 =  (OperatorToken*)infixLed((Token*)subToken);
  TEST_ASSERT_EQUAL(TOKEN_UNKNOWN_TYPE, subToken->type);
  TEST_ASSERT_EQUAL("s", subToken->symbol);
  TEST_ASSERT_EQUAL(INFIX, subToken->arity);
  TEST_ASSERT_EQUAL(0, subToken->bindingPower);
}