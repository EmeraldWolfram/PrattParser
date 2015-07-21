#include "unity.h"
#include <stdlib.h>
#include "Token.h"
#include "mock_ErrorObject.h"
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

