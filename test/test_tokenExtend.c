#include "unity.h"
#include <stdlib.h>
#include "Token.h"
#include "ErrorObject.h"
#include "tokenExtend.h"
#include "mock_parser.h"


ErrorObject* err;
OperatorAttributes operatorAttributesTable1[] = {
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

/**
 *  The extendSingleCharacterOperator function will assign attribute from the
 *  attribute table to the token according to the given symbol.
 *
 *  Given the token '~' the attribute should be assign as shown below
 *
 *  [Type: OperatorToken* ]           [Type: OperatorToken* ]
 *  [name: invert         ]           [name: invert         ]
 *  [symbol: "~"          ]           [symbol: "~"          ]
 *  [Arity: INFIX         ]     *->   [Arity: PREFIX        ]
 *                              *->   [bindingPower: 70     ]
 *                              *->   [nud: infixNud        ]
 *                              *->   [led: infixLed        ]
 *                              *->   [extend: Single       ]
 *
 */
void test_extendSingleCharacterOperator_given_INVERT_and_Table_should_assign_70_PREFIX(void){
  OperatorToken* invert   = (OperatorToken*)createOperatorToken("~", INFIX);
  int thisSymbol          = (int)(*invert->symbol);
  OperatorAttributes oprAttr  = operatorAttributesTable1[thisSymbol];
  
  invert = (OperatorToken*)oprAttr.extend((Token*)invert, &oprAttr);
  
  TEST_ASSERT_EQUAL(PREFIX, invert->arity);
  TEST_ASSERT_EQUAL(70, invert->bindingPower);
}

/**
 *  The extendSingleCharacterOperator function will assign attribute from the
 *  attribute table to the token according to the given symbol.
 *
 *  Given the token '~*' the attribute should be throw ERR_UNDEFINED_OPERATOR
 *  because '~*' is not a defined operator
 *
 */
void test_extendSingleCharacterOperator_given_INVERT_MUL_and_Table_should_throw_ERR_UNDEFINED_OPERATOR(void){
  OperatorToken* invert   = (OperatorToken*)createOperatorToken("~*", INFIX);
  int thisSymbol          = (int)(*invert->symbol);
  OperatorAttributes oprAttr  = operatorAttributesTable1[thisSymbol];
  
  Try{
    invert = (OperatorToken*)oprAttr.extend((Token*)invert, &oprAttr);
    TEST_FAIL_MESSAGE("Expected ERR_UNDEFINED_OPERATOR but not error were thrown");
  }Catch(err){
    TEST_ASSERT_EQUAL(ERR_UNDEFINED_OPERATOR, err->errorCode);
    TEST_ASSERT_EQUAL_STRING("This operator is undefined!", err->errorMsg);
  }
}
/**
 *  The extendDoubleCharacterOperator function will assign attribute to operator
 *  can be two different operator function.
 *  
 *  Example,
 *    extendDouble differentiate '/' from '/='
 *    then assign attribute to '/' from the attribute table.
 *
 *  Given the token '/' the attribute should be assign as shown below
 *
 *  [Type: OperatorToken* ]           [Type: OperatorToken* ]
 *  [name: div            ]           [name: div            ]
 *  [symbol: "/"          ]           [symbol: "/"          ]
 *  [Arity: INFIX         ]     *->   [Arity: INFIX         ]
 *                              *->   [bindingPower: 60     ]
 *                              *->   [nud: infixNud        ]
 *                              *->   [led: infixLed        ]
 *                              *->   [extend: Double       ]
 *
 */
void test_extendDoubleCharacterOperator_given_DIV_and_Table_should_assign_60_INFIX(void){
  OperatorToken* div  = (OperatorToken*)createOperatorToken("/", INFIX);
  int thisSymbol      = (int)(*div->symbol);
  OperatorAttributes oprAttr  = operatorAttributesTable1[thisSymbol];
  
  div = (OperatorToken*)oprAttr.extend((Token*)div, &oprAttr);
  
  TEST_ASSERT_EQUAL(INFIX, div->arity);
  TEST_ASSERT_EQUAL(60, div->bindingPower);
}
/**
 *  The extendDoubleCharacterOperator function will assign attribute to operator
 *  can be two different operator function.
 *  
 *  Given the token '*+', extendDoubleCharacterOperator should throw ERR_UNDEFINED_OPERATOR
 *
 */
void test_extendDoubleCharacterOperator_given_MUL_ADD_and_Table_should_throw_ERR_UNDEFINED_OPERATOR(void){
  OperatorToken* mul  = (OperatorToken*)createOperatorToken("*+", INFIX);
  int thisSymbol      = (int)(*mul->symbol);
  OperatorAttributes oprAttr  = operatorAttributesTable1[thisSymbol];
  
  Try{
    mul = (OperatorToken*)oprAttr.extend((Token*)mul, &oprAttr);
    TEST_FAIL_MESSAGE("Expected ERR_UNDEFINED_OPERATOR but not error were thrown");
  }Catch(err){
    TEST_ASSERT_EQUAL(ERR_UNDEFINED_OPERATOR, err->errorCode);
    TEST_ASSERT_EQUAL_STRING("This operator is undefined!", err->errorMsg);
  }
}
/**
 *  The extendTripleCharacterOperator function will assign attribute to operator
 *  can be 3 different operator function.
 *  
 *  Example,
 *    extendTriple differentiate '++' from '+' and '+='
 *    then assign attribute to '++' from the attribute table.
 *
 *  Given the token '++' the attribute should be assign as shown below
 *
 *  [Type: OperatorToken* ]           [Type: OperatorToken* ]
 *  [name: incr           ]           [name: incr           ]
 *  [symbol: "++"         ]           [symbol: "++"         ]
 *  [Arity: INFIX         ]     *->   [Arity: PREFIX        ]
 *                              *->   [bindingPower: 70     ]
 *                              *->   [nud: infixNud        ]
 *                              *->   [led: infixLed        ]
 *                              *->   [extend: Triple       ]
 *
 */
void test_extendTripleCharacterOperator_given_INCR_and_Table_should_assign_70_PREFIX(void){
  OperatorToken* incr  = (OperatorToken*)createOperatorToken("++", INFIX);
  int thisSymbol      = (int)(*incr->symbol);
  OperatorAttributes oprAttr  = operatorAttributesTable1[thisSymbol];
  
  incr = (OperatorToken*)oprAttr.extend((Token*)incr, &oprAttr);
  
  TEST_ASSERT_EQUAL(PREFIX, incr->arity);
  TEST_ASSERT_EQUAL(70, incr->bindingPower);
}
/**
 *  The extendTripleCharacterOperator function will assign attribute to operator
 *  can be 3 different operator function.
 *  
 *  Given the token '++^', extendTripleCharacterOperator should throw ERR_UNDEFINED_OPERATOR
 *
 */
void test_extendTripleCharacterOperator_given_INCR_XOR_and_Table_should_throw_ERR_UNDEFINED_OPERATOR(void){
  OperatorToken* incr = (OperatorToken*)createOperatorToken("++^", INFIX);
  int thisSymbol      = (int)(*incr->symbol);
  OperatorAttributes oprAttr  = operatorAttributesTable1[thisSymbol];
  
  Try{
    incr = (OperatorToken*)oprAttr.extend((Token*)incr, &oprAttr);
    TEST_FAIL_MESSAGE("Expected ERR_UNDEFINED_OPERATOR but not error were thrown");
  }Catch(err){
    TEST_ASSERT_EQUAL(ERR_UNDEFINED_OPERATOR, err->errorCode);
    TEST_ASSERT_EQUAL_STRING("This operator is undefined!", err->errorMsg);
  }
}
/**
 *  The extendQuadrupleCharacterOperator function will assign attribute to operator
 *  can be 4 different operator function.
 *  
 *  Example,
 *    extendTriple differentiate '>>' from '>', '>>=' and '>='
 *    then assign attribute to '>>' from the attribute table.
 *
 *  Given the token '>>' the attribute should be assign as shown below
 *
 *  [Type: OperatorToken* ]           [Type: OperatorToken* ]
 *  [name: rotate         ]           [name: rotate         ]
 *  [symbol: ">>"         ]           [symbol: ">>"         ]
 *  [Arity: INFIX         ]           [Arity: INFIX         ]
 *                              *->   [bindingPower: 40     ]
 *                              *->   [nud: infixNud        ]
 *                              *->   [led: infixLed        ]
 *                              *->   [extend: Quadruple    ]
 *
 */
void test_extendQuadrupleCharacterOperator_given_ROTATE_and_Table_should_assign_70_INFIX(void){
  OperatorToken* rotate   = (OperatorToken*)createOperatorToken(">>", INFIX);
  int thisSymbol          = (int)(*rotate->symbol);
  OperatorAttributes oprAttr  = operatorAttributesTable1[thisSymbol];
  
  rotate = (OperatorToken*)oprAttr.extend((Token*)rotate, &oprAttr);
  
  TEST_ASSERT_EQUAL(INFIX, rotate->arity);
  TEST_ASSERT_EQUAL(40, rotate->bindingPower);
}
/**
 *  The extendQuadrupleCharacterOperator function will assign attribute to operator
 *  can be 4 different operator function.
 *  
 *  Given the token '>>=4', extendTripleCharacterOperator should throw ERR_UNDEFINED_OPERATOR
 *
 */
void test_extendQuadrupleCharacterOperator_given_ROTATE_EQUAL_4_and_Table_should_throw_ERR_UNDEFINED_OPERATOR(void){
  OperatorToken* rotate = (OperatorToken*)createOperatorToken(">>=4", INFIX);
  int thisSymbol      = (int)(*rotate->symbol);
  OperatorAttributes oprAttr  = operatorAttributesTable1[thisSymbol];
  
  Try{
    rotate = (OperatorToken*)oprAttr.extend((Token*)rotate, &oprAttr);
    TEST_FAIL_MESSAGE("Expected ERR_UNDEFINED_OPERATOR but not error were thrown");
  }Catch(err){
    TEST_ASSERT_EQUAL(ERR_UNDEFINED_OPERATOR, err->errorCode);
    TEST_ASSERT_EQUAL_STRING("This operator is undefined!", err->errorMsg);
  }
}
/*  createOperatorToken("*", INFIX) should create a pointer to OperatorToken
 *
 *                                     [OperatorToken]
 *  [OperatorToken ptr] -------------> [      *      ]
 *                                     [    PREFIX   ]
 *
 *  When this token sub into infixNud,
 *  this test will return     (*)  
 *                            /
 *                          (4)
 */
void test_infixNud_given_Token_of_Prefix_symbol_MUL_should_return_MUL_4(void){
  OperatorToken* mulToken = (OperatorToken*)createOperatorToken("*", INFIX);
  IntegerToken* intToken4 = (IntegerToken*)createIntegerToken(4);
  parser_ExpectAndReturn(100, (Token*)intToken4);
  
  mulToken =  (OperatorToken*)infixNud((Token*)mulToken);
  TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE, mulToken->type);
  TEST_ASSERT_EQUAL("*", mulToken->symbol);
  TEST_ASSERT_EQUAL(PREFIX, mulToken->arity);
  TEST_ASSERT_EQUAL(60, mulToken->bindingPower);
}

/*  createOperatorToken("/", INFIX) should create a pointer to OperatorToken
 *
 *                                     [OperatorToken]
 *  [OperatorToken ptr] -------------> [      /      ]
 *                                     [    INFIX    ]
 *
 *  When this token sub into infixNud,
 *  this test will show error ILLEGAL PREFIX
 */
void test_infixNud_given_Token_of_DIV_should_show_error_msg(void){
  OperatorToken* divToken = (OperatorToken*)createOperatorToken("/", INFIX);
  Try{
    divToken =  (OperatorToken*)infixNud((Token*)divToken);
    TEST_FAIL_MESSAGE("Expecting ERR_ILLEGAL_PREFIX but none thrown.");
  }Catch(err){
    TEST_ASSERT_EQUAL(ERR_ILLEGAL_PREFIX, err->errorCode);
    TEST_ASSERT_EQUAL_STRING("This is an illegal prefix!", err->errorMsg);
    freeError(err);
  }
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
  OperatorToken* subToken = (OperatorToken*)createOperatorToken("s", INFIX);
  
  Try{
    subToken =  (OperatorToken*)infixLed((Token*)subToken);
    TEST_FAIL_MESSAGE("Expecting ERR_ILLEGAL_CHARACTER but none thrown.");
  }Catch(err){
    TEST_ASSERT_EQUAL(ERR_ILLEGAL_CHARACTER, err->errorCode);
    TEST_ASSERT_EQUAL_STRING("This is illegal character!", err->errorMsg);
    freeError(err);
  }
}

void test_infixLed_given_Token_of_Infix_symbol_3_should_show_error_msg(void){
  OperatorToken* subToken = (OperatorToken*)createOperatorToken("3", INFIX);
  
  Try{
    subToken =  (OperatorToken*)infixLed((Token*)subToken);
    TEST_FAIL_MESSAGE("Expecting ERR_UNEXPECTED_INTEGER but none thrown.");
  }Catch(err){
    TEST_ASSERT_EQUAL(ERR_UNEXPECTED_INTEGER, err->errorCode);
    TEST_ASSERT_EQUAL_STRING("Integer unexpected to be here!",  err->errorMsg);
    freeError(err);
  }
}






