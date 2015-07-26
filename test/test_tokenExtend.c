#include "unity.h"
#include <stdlib.h>
#include "Token.h"
#include "ErrorObject.h"
#include "tokenExtend.h"
#include "mock_parser.h"

ErrorObject* err;
extern Attributes operatorAttributesTable[];
extern Attributes tokenTypeAttributesTable[];

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
 *                              *->   [nud: prefixNud       ]
 *                              *->   [led: infixLed        ]
 *
 */
void test_extendSingleCharacterOperator_given_INVERT_and_Table_should_assign_70_PREFIX(void){
  OperatorToken* invert   = (OperatorToken*)createOperatorToken("~", INFIX);
  int thisSymbol          = *invert->symbol;
  Attributes oprAttr  = operatorAttributesTable[thisSymbol];
  
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
  int thisSymbol          = *invert->symbol;
  Attributes oprAttr  = operatorAttributesTable[thisSymbol];
  
  Try{
    invert = (OperatorToken*)oprAttr.extend((Token*)invert, &oprAttr);
    TEST_FAIL_MESSAGE("Expected ERR_UNDEFINED_OPERATOR but no error were thrown");
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
 *                              *->   [nud: prefixNud       ]
 *                              *->   [led: infixLed        ]
 *
 */
void test_extendDoubleCharacterOperator_given_DIV_and_Table_should_assign_60_INFIX(void){
  OperatorToken* div  = (OperatorToken*)createOperatorToken("/", INFIX);
  int thisSymbol      = *div->symbol;
  Attributes oprAttr  = operatorAttributesTable[thisSymbol];
  
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
  int thisSymbol      = *mul->symbol;
  Attributes oprAttr  = operatorAttributesTable[thisSymbol];
  
  Try{
    mul = (OperatorToken*)oprAttr.extend((Token*)mul, &oprAttr);
    TEST_FAIL_MESSAGE("Expected ERR_UNDEFINED_OPERATOR but no error were thrown");
  }Catch(err){
    TEST_ASSERT_EQUAL(ERR_UNDEFINED_OPERATOR, err->errorCode);
    TEST_ASSERT_EQUAL_STRING("This operator is undefined!", err->errorMsg);
  }
}


/**
 *  The extendDoubleCharacterOperator function will assign attribute to operator
 *  can be two different operator function.
 *  
 *	Operator '*' can only be extended as '*' & '*='.  
 *  So given the token '*=+', extendDoubleCharacterOperator should throw ERR_UNDEFINED_OPERATOR
 *
 */
void test_extendDoubleCharacterOperator_given_MUL_EQUAL_and_ADD_should_throw_ERR_UNDEFINED_OPERATOR(void){
  OperatorToken* opr  = (OperatorToken*)createOperatorToken("*=+", INFIX);
  int thisSymbol      = *opr->symbol;
  Attributes oprAttr  = operatorAttributesTable[thisSymbol];
  
  Try{
    opr = (OperatorToken*)oprAttr.extend((Token*)opr, &oprAttr);
    TEST_FAIL_MESSAGE("Expected ERR_UNDEFINED_OPERATOR but no error were thrown");
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
 *                              *->   [nud: prefixNud       ]
 *                              *->   [led: infixLed        ]
 *
 */
void test_extendTripleCharacterOperator_given_INCR_and_Table_should_assign_70_PREFIX(void){
  OperatorToken* incr = (OperatorToken*)createOperatorToken("++", INFIX);
  int thisSymbol      = *incr->symbol;
  Attributes oprAttr  = operatorAttributesTable[thisSymbol];
  
  incr = (OperatorToken*)oprAttr.extend((Token*)incr, &oprAttr);
  
  TEST_ASSERT_EQUAL(PREFIX, incr->arity);
  TEST_ASSERT_EQUAL(70, incr->bindingPower);
}

/**
 *  The extendTripleCharacterOperator function will assign attribute to operator
 *  can be 3 different operator function.
 *  
 *  Example,
 *    extendTriple differentiate '++' from '+' and '+='
 *	
 *  Given the token '+/', extendTripleCharacterOperator should throw ERR_UNDEFINED_OPERATOR
 *
 */
void test_extendTripleCharacterOperator_given_ADD_DIV_should_throw_ERR_UNDEFINED_OPERATOR(void){
  OperatorToken* opr = (OperatorToken*)createOperatorToken("+/", INFIX);
  int thisSymbol      = *opr->symbol;
  Attributes oprAttr  = operatorAttributesTable[thisSymbol];
  
  Try{
    opr = (OperatorToken*)oprAttr.extend((Token*)opr, &oprAttr);
    TEST_FAIL_MESSAGE("Expected ERR_UNDEFINED_OPERATOR but no error were thrown");
  }Catch(err){
    TEST_ASSERT_EQUAL(ERR_UNDEFINED_OPERATOR, err->errorCode);
    TEST_ASSERT_EQUAL_STRING("This operator is undefined!", err->errorMsg);
  }
}

/**
 *  The extendTripleCharacterOperator function will assign attribute to operator
 *  can be 3 different operator function.
 *  
 *  Given the token '++^', extendTripleCharacterOperator should throw ERR_UNDEFINED_OPERATOR
 *
 */
void test_extendTripleCharacterOperator_given_INCR_XOR_should_throw_ERR_UNDEFINED_OPERATOR(void){
  OperatorToken* incr = (OperatorToken*)createOperatorToken("++^", INFIX);
  int thisSymbol      = *incr->symbol;
  Attributes oprAttr  = operatorAttributesTable[thisSymbol];
  
  Try{
    incr = (OperatorToken*)oprAttr.extend((Token*)incr, &oprAttr);
    TEST_FAIL_MESSAGE("Expected ERR_UNDEFINED_OPERATOR but no error were thrown");
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
 *    extendQuadruple differentiate '>>' from '>', '>>=' and '>='
 *    then assign attribute to '>>' from the attribute table.
 *
 *  Given the token '>>' the attribute should be assign as shown below
 *
 *  [Type: OperatorToken* ]           [Type: OperatorToken* ]
 *  [name: rotate         ]           [name: rotate         ]
 *  [symbol: ">>"         ]           [symbol: ">>"         ]
 *  [Arity: INFIX         ]           [Arity: INFIX         ]
 *                              *->   [bindingPower: 40     ]
 *                              *->   [nud: prefixNud       ]
 *                              *->   [led: infixLed        ]
 *            
 *
 */
void test_extendQuadrupleCharacterOperator_given_ROTATE_and_Table_should_assign_70_INFIX(void){
  OperatorToken* rotate   = (OperatorToken*)createOperatorToken(">>", INFIX);
  int thisSymbol          = *rotate->symbol;
  Attributes oprAttr  = operatorAttributesTable[thisSymbol];
  
  rotate = (OperatorToken*)oprAttr.extend((Token*)rotate, &oprAttr);
  
  TEST_ASSERT_EQUAL(INFIX, rotate->arity);
  TEST_ASSERT_EQUAL(40, rotate->bindingPower);
}

/**
 *  The extendQuadrupleCharacterOperator function will assign attribute to operator
 *  can be 4 different operator function.
 *
 *	Example,
 *		extendQuadruple differentiate '>>' from '>', '>>=' and '>='
 *  
 *  Therefore, given the token '>+', extendQuadrupleCharacterOperator should throw ERR_UNDEFINED_OPERATOR
 *
 */
void test_extendQuadrupleCharacterOperator_given_LARGER_ADD_and_Table_should_throw_ERR_UNDEFINED_OPERATOR(void){
  OperatorToken* larger = (OperatorToken*)createOperatorToken(">+", INFIX);
  int thisSymbol      = *larger->symbol;
  Attributes oprAttr  = operatorAttributesTable[thisSymbol];
  
  Try{
    larger = (OperatorToken*)oprAttr.extend((Token*)larger, &oprAttr);
    TEST_FAIL_MESSAGE("Expected ERR_UNDEFINED_OPERATOR but no error were thrown");
  }Catch(err){
    TEST_ASSERT_EQUAL(ERR_UNDEFINED_OPERATOR, err->errorCode);
    TEST_ASSERT_EQUAL_STRING("This operator is undefined!", err->errorMsg);
  }
}

/**
 *  The extendQuadrupleCharacterOperator function will assign attribute to operator
 *  can be 4 different operator function.
 *  
 *  Given the token '>>=4', extendQuadrupleCharacterOperator should throw ERR_UNDEFINED_OPERATOR
 *
 */
void test_extendQuadrupleCharacterOperator_given_ROTATE_EQUAL_4_and_Table_should_throw_ERR_UNDEFINED_OPERATOR(void){
  OperatorToken* rotate = (OperatorToken*)createOperatorToken(">>=4", INFIX);
  int thisSymbol      = *rotate->symbol;
  Attributes oprAttr  = operatorAttributesTable[thisSymbol];
  
  Try{
    rotate = (OperatorToken*)oprAttr.extend((Token*)rotate, &oprAttr);
    TEST_FAIL_MESSAGE("Expected ERR_UNDEFINED_OPERATOR but no error were thrown");
  }Catch(err){
    TEST_ASSERT_EQUAL(ERR_UNDEFINED_OPERATOR, err->errorCode);
    TEST_ASSERT_EQUAL_STRING("This operator is undefined!", err->errorMsg);
  }
}

/**
 *  The extendExpression function will assign attributes to Integer Token, Float Token, Identifier Token,
 *  String Token and Operator Token.
 *  
 *  Example,
 *    extendExpression differentiate different token type then assign attribute according to attribute table.
 *
 *  Given the token with value 12, the attribute should be assign as shown below
 *
 *  [Type: IntegerToken*  ]           [Type: IntegerToken*  ]
 *  [name: intToken       ]           [name: intToken       ]
 *  [value: 12            ]           [value: 12            ]
 *  [nud: NULL            ]     *->   [nud: expressionNud   ]
 *  [led: NULL            ]     *->   [led: errorLed        ]
 *               
 *
 */
void test_extendExpression_given_IntegerToken_and_Table_should_assign_expressionNud_and_errorLed(void){
  IntegerToken* intToken  = (IntegerToken*)createIntegerToken(12);
  Attributes intAttr      = tokenTypeAttributesTable[intToken->type];
  intToken                = (IntegerToken*)intAttr.extend((Token*)intToken, &intAttr);
  
  TEST_ASSERT_EQUAL(intToken->nud, expressionNud);
  TEST_ASSERT_EQUAL(intToken->led, errorLed);
}

/**
 *  The extendExpression function will assign attributes to Integer Token, Float Token, Identifier Token,
 *  String Token and Operator Token.
 *  
 *  Example,
 *    extendExpression differentiate different token type then assign attribute according to attribute table.
 *
 *  Given the OperatorToken '%', the function will call extend  (extendDoubleCharacterOperator in this case) 
 *  function to assign attributes.
 *
 *  [Type: OperatorToken* ]           [Type: OperatorToken* ]
 *  [name: oprToken       ]           [name: oprToken       ]
 *  [symbol: '%'          ]           [symbol: '%'          ]
 *  [bindingPower: NULL   ]     *->   [bindingPower: 60     ]
 *  [arity: INFIX         ]           [arity: INFIX         ]
 *  [nud: NULL            ]     *->   [nud: errorNud        ]
 *  [led: NULL            ]     *->   [led: infixLed        ]
 *               
 *
 */
void test_extendExpression_given_OperatorToken_REMAINDER_and_Table_should_assign_errorNud_and_infixLed(void){
  OperatorToken* oprToken  = (OperatorToken*)createOperatorToken("%", INFIX);
  Attributes  oprAttr      = tokenTypeAttributesTable[oprToken->type];
  oprToken                 = (OperatorToken*)extendExpression((Token*)oprToken, &oprAttr);
  
  TEST_ASSERT_EQUAL(oprToken->nud, errorNud);
  TEST_ASSERT_EQUAL(oprToken->led, infixLed);
  TEST_ASSERT_EQUAL(60, oprToken->bindingPower);
}

/**
 *  The extendExpression function will assign attributes to Integer Token, Float Token, Identifier Token,
 *  String Token and Operator Token.
 *  
 *  Example,
 *    extendExpression differentiate different token type then assign attribute according to attribute table.
 *
 *  Given an unknown type token should throw ERR_UNKNOWN_TOKEN
 */
void test_extendExpression_given_Unknown_Token_and_Table_should_throw_ERR_UNKNOWN_TOKEN(void){
  Token* testToken  = malloc(sizeof(Token));
  testToken->type   = TOKEN_UNKNOWN_TYPE;
  
  Try{
    Attributes attr   = tokenTypeAttributesTable[TOKEN_UNKNOWN_TYPE];
    testToken         = extendExpression(testToken, &attr);
    TEST_FAIL_MESSAGE("Expected ERR_UNKNOWN_TOKEN but no error were thrown");
  } Catch(err){
    TEST_ASSERT_EQUAL(ERR_UNKNOWN_TOKEN, err->errorCode);
    TEST_ASSERT_EQUAL_STRING("Can't resolve Unknown type token!", err->errorMsg);    
  }
}

/**
 *  extendErrorOperator is a function assign to possible undefined operator
 *  The function will always throw ERR_UNDEFINED_OPERATOR when called
 *
 *  Given an operator token should throw ERR_UNDEFINED_OPERATOR
 */
void test_extendErrorOperator_should_always_throw_ERR_UNDEFINED_OPERATOR(void){
  OperatorToken* testToken  = (OperatorToken*)createOperatorToken("@", INFIX);

  Try{
    Attributes attr = tokenTypeAttributesTable[testToken->type];
    testToken = (OperatorToken*)attr.extend((Token*)testToken, &attr);
    TEST_FAIL_MESSAGE("Expected ERR_UNDEFINED_OPERATOR but no error were thrown");
  } Catch(err){
    TEST_ASSERT_EQUAL(ERR_UNDEFINED_OPERATOR, err->errorCode);
    TEST_ASSERT_EQUAL_STRING("This operator is undefined!", err->errorMsg);    
  }
}

/**
 *  extendCharaterErrorOperator is a function that will throw ERR_ILLEGAL_CHARACTER
 *  when an operator type token that have any symbol from 'a' to 'z' including uppercase,
 *
 */
void test_extendCharacterErrorOperator_should_always_throw_ERR_ILLEGAL_CHARACTER(void){
  OperatorToken* testToken  = (OperatorToken*)createOperatorToken("G", INFIX);

  Try{
    Attributes attr = tokenTypeAttributesTable[testToken->type];
    testToken = (OperatorToken*)attr.extend((Token*)testToken, &attr);
    TEST_FAIL_MESSAGE("Expected ERR_ILLEGAL_CHARACTER but no error were thrown");
  } Catch(err){
    TEST_ASSERT_EQUAL(ERR_ILLEGAL_CHARACTER, err->errorCode);
    TEST_ASSERT_EQUAL_STRING("Character operator is illegal!", err->errorMsg);    
  }
}

/**
 *  extendIntegerErrorOperator is a function that will throw ERR_ILLEGAL_INTEGER
 *  when an operator type token that have any digit symbol is detected.
 *
 */
void test_extendIntegerErrorOperator_should_always_throw_ERR_ILLEGAL_INTEGER(void){
  OperatorToken* testToken  = (OperatorToken*)createOperatorToken("76", INFIX);

  Try{
    Attributes attr = tokenTypeAttributesTable[testToken->type];
    testToken = (OperatorToken*)attr.extend((Token*)testToken, &attr);
    TEST_FAIL_MESSAGE("Expected ERR_ILLEGAL_INTEGER but no error were thrown");
  } Catch(err){
    TEST_ASSERT_EQUAL(ERR_ILLEGAL_INTEGER, err->errorCode);
    TEST_ASSERT_EQUAL_STRING("Integer operator is illegal!", err->errorMsg);    
  }
}





