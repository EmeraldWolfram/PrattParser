#include "tokenExtend.h"
#include "Token.h"
#include "execute.h"
#include "ErrorObject.h"
#include "CException.h"
#include "parser.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//this table defined the details of the operator token. 
Attributes operatorAttributesTable[] = {
  ['<'] =         {30,	INFIX,  errorNud,  infixLed, extendQuadrupleCharacterOperator, executeDouble},
  ['>'] =         {30,  INFIX,  errorNud,  infixLed, extendQuadrupleCharacterOperator, executeDouble},
  ['+'] =         {50,  PREFIX, prefixNud, infixLed, extendTripleCharacterOperator, executeSingle},
  ['-'] =         {50,  PREFIX, prefixNud, infixLed, extendTripleCharacterOperator, executeSingle},
  ['&'] =         {10,  PREFIX, prefixNud, infixLed, extendTripleCharacterOperator, executeSingle},
  ['|'] =         { 8,  INFIX,  errorNud,  infixLed, extendTripleCharacterOperator, executeDouble},
  ['*'] =         {60,  PREFIX, prefixNud, infixLed, extendDoubleCharacterOperator, executeSingle},
  ['/'] =         {60,  INFIX,  errorNud,  infixLed, extendDoubleCharacterOperator, executeDouble},
  ['%'] =         {60,  INFIX,  errorNud,  infixLed, extendDoubleCharacterOperator, executeDouble},
  ['^'] =         { 9,  INFIX,  errorNud,  infixLed, extendDoubleCharacterOperator, executeDouble},
  ['!'] =         {70,  PREFIX, prefixNud, infixLed, extendDoubleCharacterOperator, executeSingle},
  ['='] =         { 5,  INFIX,  errorNud,  infixLed, extendDoubleCharacterOperator, executeDouble},
  ['~'] =         {70,  PREFIX, prefixNud, errorLed, extendSingleCharacterOperator, executeSingle},
  ['('] =         { 1,  PREFIX, prefixNud, infixLed, extendSingleCharacterOperator, executeSingle},
  [')'] =         { 0,  NOFIX,  prefixNud, infixLed, extendSingleCharacterOperator, executeDouble},
  ['['] =         { 1,  PREFIX, prefixNud, infixLed, extendSingleCharacterOperator, executeSingle},
  [']'] =         { 0,  NOFIX,  prefixNud, infixLed, extendSingleCharacterOperator, executeDouble},
  ['$'] =         { 0,  NOFIX,  errorNud,  infixLed, extendSingleCharacterOperator, executeDouble},
  ['#'] =         { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator, executeDouble},
  ['{'] =         { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator, executeDouble},
  ['}'] =         { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator, executeDouble},
  ['@'] =         { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator, executeDouble},
  ['?'] =         { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator, executeDouble},
  ['.'] =         { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator, executeDouble},
  [','] =         { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator, executeDouble},
  [';'] =         { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator, executeDouble},
  ['"'] =         { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator, executeDouble},
  ['\'']=         { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator, executeDouble},
  ['`'] =         { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator, executeDouble},
  ['\\']=         { 1,	NOFIX,  errorNud,  errorLed, extendErrorOperator, executeDouble},
  ['a' ... 'z'] = { 1,	NOFIX, 	errorNud,  errorLed, extendCharacterErrorOperator, executeDouble},
  ['A' ... 'Z'] = { 1,	NOFIX,	errorNud,  errorLed, extendCharacterErrorOperator, executeDouble},
  [48  ...  57] = { 1,	NOFIX,  errorNud,  errorLed, extendIntegerErrorOperator, executeDouble}
};

//this table defined the details of the token types. 
Attributes  tokenTypeAttributesTable[] = {
  [TOKEN_OPERATOR_TYPE]   = {0, NOFIX, prefixNud, infixLed, extendExpression, executeDouble},
  [TOKEN_UNKNOWN_TYPE]    = {0, NOFIX, errorNud,  errorLed, extendExpression, executeExpression},
  [TOKEN_INTEGER_TYPE]    = {0, NOFIX, expressionNud, errorLed, extendExpression, executeExpression},
  [TOKEN_FLOAT_TYPE]      = {0, NOFIX, expressionNud, errorLed, extendExpression, executeExpression},
  [TOKEN_STRING_TYPE]     = {0, NOFIX, expressionNud, errorLed, extendExpression, executeExpression},
  [TOKEN_IDENTIFIER_TYPE] = {0, NOFIX, expressionNud, errorLed, extendExpression, executeExpression}
};

/*Function of prefixNud is to grab the operator token.
 *	If the token is not operator, then return the token.
 *	
 */
Token* prefixNud(Token* myself){
  assert(myself != NULL);
  myself->execute   = executeSingle;
  if(strcmp(((OperatorToken*)myself)->symbol,"(") != 0 \
  && strcmp(((OperatorToken*)myself)->symbol,"[") != 0)
    ((OperatorToken*)myself)->token[0] = parser(100);
  else{
    ((OperatorToken*)myself)->token[0] = parser(1);
  }
  return myself;
}

/* When Nud is called and the Token is not an operatorToken,
 * 		expressionNud will return the Token.
 *
 * expressionNud is to prevent error.
 * 		When an expressionToken passed into parser, 
 *		expressionNud will direct return the Token,
 *		without checking the symbol.
 */
Token* expressionNud(Token* myself){
  assert(myself != NULL);
  myself->execute = executeExpression;
  return myself;
}

/* 
 * If the OperatorToken that passed into Nud is not prefix,
 *		the errorNud will be called,
 *		errorCode: ERR_ILLEGAL_PREFIX and errorMsg:"This is an illegal prefix!"  will be throw.
 *
 */
Token* errorNud(Token* myself){
  assert(myself != NULL);
  ThrowError(ERR_ILLEGAL_PREFIX, "ERROR: '%s' is an illegal prefix!", ((OperatorToken*)myself)->symbol);
  return myself;
}

// infixLed will assign INFIX or POSTFIX to the arity of OperatorToken.
Token* infixLed(Token* myself){
  assert(myself != NULL);
  
  if(strcmp(((OperatorToken*)myself)->symbol,"++") == 0 \
  || strcmp(((OperatorToken*)myself)->symbol,"--") == 0){
    ((OperatorToken*)myself)->arity = POSTFIX;
    myself->execute = executeSingle;
  }
  else{
    ((OperatorToken*)myself)->arity = INFIX;
    myself->execute = executeDouble;
  }  
  return myself;
}

/* if expression token passed into led,
 * 		errorLed will be called,
 *		errorCode: ERR_UNEXPECTED_EXPRESSION and errorMsg:"Expected operator token but obtained expression!"  will be throw.
 */
Token* errorLed(Token* myself){
  assert(myself != NULL);
  switch(myself->type){
    case TOKEN_INTEGER_TYPE:
      ThrowError(ERR_UNEXPECTED_EXPRESSION, "Expected operator token but obtained integer %d!", ((IntegerToken*)myself)->value);
      break;
    case TOKEN_FLOAT_TYPE:
      ThrowError(ERR_UNEXPECTED_EXPRESSION, "Expected operator token but obtained float %f!", ((FloatToken*)myself)->value);
      break;
    case TOKEN_STRING_TYPE:
      ThrowError(ERR_UNEXPECTED_EXPRESSION, "Expected operator token but obtained string %s!", ((StringToken*)myself)->name);
      break;
    case TOKEN_IDENTIFIER_TYPE:
      ThrowError(ERR_UNEXPECTED_EXPRESSION, "Expected operator token but obtained identifier %s!", ((StringToken*)myself)->name);
      break;
  }
  return myself;
}

/*	The operators that only have one function,
 *	will call extendSingleCharacterOperator.
 *	
 *	Example: '~', '~', '(', ')', '[', ']' etc.
 */
Token* extendSingleCharacterOperator(Token* thisOpr, Attributes *attr){
  if(((OperatorToken*)thisOpr)->symbol[1] == 0){
    ((OperatorToken*)thisOpr)->bindingPower  = attr->bindingPower;
    ((OperatorToken*)thisOpr)->arity         = attr->arity;
    ((OperatorToken*)thisOpr)->nud           = attr->nud;
    ((OperatorToken*)thisOpr)->led           = attr->led;
    ((OperatorToken*)thisOpr)->execute       = attr->execute;
  }
  else
    ThrowError(ERR_UNDEFINED_OPERATOR, "ERROR: Operator '%s' is undefined!", ((OperatorToken*)thisOpr)->symbol);  //Throw error such as "~X", where X can be any character
  return thisOpr;
}


/*	The operators that have two functions,
 *	will call extendDoubleCharacterOperator.
 *	
 *	Example: ('*', '*='), ('/', '='), ('%', '%=') etc.
 */
Token* extendDoubleCharacterOperator(Token *thisOpr, Attributes *attr){ 
  ((OperatorToken*)thisOpr)->nud           = attr->nud;
  ((OperatorToken*)thisOpr)->led           = attr->led;
  ((OperatorToken*)thisOpr)->execute       = attr->execute;
  
  if(((OperatorToken*)thisOpr)->symbol[1] == 0){
    ((OperatorToken*)thisOpr)->bindingPower  = attr->bindingPower;
    ((OperatorToken*)thisOpr)->arity         = attr->arity;
  }
  else if(((OperatorToken*)thisOpr)->symbol[1] == '='){
    if(((OperatorToken*)thisOpr)->symbol[2] == 0){
      ((OperatorToken*)thisOpr)->bindingPower  = 5;
      ((OperatorToken*)thisOpr)->arity         = INFIX;
    }
    else
      ThrowError(ERR_UNDEFINED_OPERATOR, "ERROR: Operator '%s' is undefined!", ((OperatorToken*)thisOpr)->symbol); //Throw error such as "*=X" detected, where X can be any character
  }
  else
    ThrowError(ERR_UNDEFINED_OPERATOR, "ERROR: Operator '%s' is undefined!", ((OperatorToken*)thisOpr)->symbol); //Throw error when "*X" detected, where X can be any character
  
  if(strcmp(((OperatorToken*)thisOpr)->symbol, "==") == 0)
    ((OperatorToken*)thisOpr)->bindingPower = 20;  
    
  return thisOpr;
}

/*	The operators that have three functions,
 *	will call extendTripleCharacterOperator.
 *	
 *	Example: ('+', '+=', '++'), ('-', '--', '-=') etc.
 */
Token* extendTripleCharacterOperator(Token *thisOpr, Attributes *attr){
  ((OperatorToken*)thisOpr)->nud           = attr->nud;
  ((OperatorToken*)thisOpr)->led           = attr->led;
  ((OperatorToken*)thisOpr)->arity         = attr->arity;
  
  if(((OperatorToken*)thisOpr)->symbol[1] == '='){
    if(((OperatorToken*)thisOpr)->symbol[2] == 0){
      ((OperatorToken*)thisOpr)->bindingPower = 5;
      ((OperatorToken*)thisOpr)->arity        = INFIX;
    }
    else
      ThrowError(ERR_UNDEFINED_OPERATOR, "ERROR: Operator '%s' is undefined!", ((OperatorToken*)thisOpr)->symbol);      //Throw error when "+=X" detected, where X can be any character
  }
  else if(((OperatorToken*)thisOpr)->symbol[1] == ((OperatorToken*)thisOpr)->symbol[0]){
    if(((OperatorToken*)thisOpr)->symbol[2] == 0)
      ((OperatorToken*)thisOpr)->bindingPower = 70;
    else
      ThrowError(ERR_UNDEFINED_OPERATOR, "ERROR: Operator '%s' is undefined!", ((OperatorToken*)thisOpr)->symbol);      //Throw error when "++X" detected, where X can be any character
  }
  else if(((OperatorToken*)thisOpr)->symbol[1] == 0)
    ((OperatorToken*)thisOpr)->bindingPower = attr->bindingPower;
  else
    ThrowError(ERR_UNDEFINED_OPERATOR, "ERROR: Operator '%s' is undefined!", ((OperatorToken*)thisOpr)->symbol);        //Throw error when "+X" detected, where X can be any character other than '=' or itself
   
  if(strcmp(((OperatorToken*)thisOpr)->symbol, "&&") == 0){
    ((OperatorToken*)thisOpr)->bindingPower = 7;
    ((OperatorToken*)thisOpr)->arity         = INFIX;
    }
  if(strcmp(((OperatorToken*)thisOpr)->symbol, "||") == 0){
    ((OperatorToken*)thisOpr)->bindingPower = 6;
  }
    
  return thisOpr;
}


/*	The operators that have four functions,
 *	will call extendQuadrupleCharacterOperator.
 *	
 *	Example: ('>', '>>', '>=', '>>=')and ('<', '<<', '<=', '<<=').
 */
Token* extendQuadrupleCharacterOperator(Token *thisOpr, Attributes *attr){
  ((OperatorToken*)thisOpr)->arity         = attr->arity;
  ((OperatorToken*)thisOpr)->nud           = attr->nud;
  ((OperatorToken*)thisOpr)->led           = attr->led;

    if(((OperatorToken*)thisOpr)->symbol[1] == 0)
      ((OperatorToken*)thisOpr)->bindingPower = attr->bindingPower;  
    else if(((OperatorToken*)thisOpr)->symbol[1] == '='){
      if(((OperatorToken*)thisOpr)->symbol[2] == 0)
        ((OperatorToken*)thisOpr)->bindingPower = attr->bindingPower;
      else
        ThrowError(ERR_UNDEFINED_OPERATOR, "ERROR: Operator '%s' is undefined!",  ((OperatorToken*)thisOpr)->symbol);        //Throw error when ">=X" detected, where X can be any character
    }
    else if(((OperatorToken*)thisOpr)->symbol[1] == ((OperatorToken*)thisOpr)->symbol[0]){
      if(((OperatorToken*)thisOpr)->symbol[2] == 0)
        ((OperatorToken*)thisOpr)->bindingPower = 40;
      else if(((OperatorToken*)thisOpr)->symbol[2] == '='){
        if(((OperatorToken*)thisOpr)->symbol[3] == 0)
          ((OperatorToken*)thisOpr)->bindingPower = 5;
        else
          ThrowError(ERR_UNDEFINED_OPERATOR, "ERROR: Operator '%s' is undefined!", ((OperatorToken*)thisOpr)->symbol);      //Throw error when ">>=X" detected, where X can be any character
      }
      else
        ThrowError(ERR_UNDEFINED_OPERATOR, "ERROR: Operator '%s' is undefined!", ((OperatorToken*)thisOpr)->symbol);        //Throw error when ">>X" detected, where X can be any character except '='
    }
    else
      ThrowError(ERR_UNDEFINED_OPERATOR, "ERROR: Operator '%s' is undefined!", ((OperatorToken*)thisOpr)->symbol);          //Throw error when ">X" detected, where X can be any character except '=' and itself
    return thisOpr;
}


/* extendExpression is to differentiate the token type,
 *		if token is unknown type,
 *		errorCode: ERR_UNKNOWN_TOKEN and errorMsg:"Can't resolve Unknown type token!"  will be throw.
 *		else return the token.
 */
Token* extendExpression(Token *thisExpression, Attributes *attr){
  if(thisExpression->type == TOKEN_UNKNOWN_TYPE)
    ThrowError(ERR_UNKNOWN_TOKEN, "Can't resolve Unknown type token!");
  else if(thisExpression->type == TOKEN_OPERATOR_TYPE){
    int thisSymbol  = *(char*)((OperatorToken*)thisExpression)->symbol;
    attr            = &operatorAttributesTable[thisSymbol];
    thisExpression  = attr->extend(thisExpression, attr);
  }
  else{
    thisExpression->execute = attr->execute;
    thisExpression->nud     = attr->nud;
    thisExpression->led     = attr->led;
  }
  return thisExpression;
}

/*	If the operatorToken is a character,
 *	errorCode: ERR_ILLEGAL_CHARACTER and errorMsg:"Character operator is illegal!"  will be throw.
 */
Token* extendCharacterErrorOperator(Token *thisOpr, Attributes *attr){
  ThrowError(ERR_ILLEGAL_CHARACTER, "Character operator '%s' is illegal!", ((OperatorToken*)thisOpr)->symbol);
}

/*	If the operatorToken is a undefined operator,
 *	errorCode: ERR_UNDEFINED_OPERATOR and errorMsg:"This operator is undefined!"  will be throw.
 */
Token* extendErrorOperator(Token *thisOpr, Attributes *attr){
  ThrowError(ERR_UNDEFINED_OPERATOR, "This operator '%s' is undefined!", ((OperatorToken*)thisOpr)->symbol);
}

/*	If the operatorToken is an integer,
 *	errorCode: ERR_ILLEGAL_INTEGER and errorMsg:"Integer operator is illegal!"  will be throw.
 */
Token* extendIntegerErrorOperator(Token *thisOpr, Attributes *attr){
  ThrowError(ERR_ILLEGAL_INTEGER, "Integer operator '%s' is illegal!", ((OperatorToken*)thisOpr)->symbol);
}


