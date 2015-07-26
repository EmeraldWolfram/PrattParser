#include "tokenExtend.h"
#include "Token.h"
#include "ErrorObject.h"
#include "CException.h"
#include "parser.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//this table defined the details of the operator token. 
Attributes operatorAttributesTable[] = {
  ['<'] = 		  {30,	INFIX,  errorNud,  infixLed, extendQuadrupleCharacterOperator},
  ['>'] = 		  {30,  INFIX,  errorNud,  infixLed, extendQuadrupleCharacterOperator},
  ['+'] =		  {50,  PREFIX, prefixNud, infixLed, extendTripleCharacterOperator},
  ['-'] = 		  {50,  PREFIX, prefixNud, infixLed, extendTripleCharacterOperator},
  ['&'] = 		  {10,  PREFIX, prefixNud, infixLed, extendTripleCharacterOperator},
  ['|'] = 		  { 8,  INFIX,  errorNud,  infixLed, extendTripleCharacterOperator},
  ['*'] = 		  {60,  PREFIX, prefixNud, infixLed, extendDoubleCharacterOperator},
  ['/'] = 		  {60,  INFIX,  errorNud,  infixLed, extendDoubleCharacterOperator},
  ['%'] = 		  {60,  INFIX,  errorNud,  infixLed, extendDoubleCharacterOperator},
  ['^'] = 		  { 9,  INFIX,  errorNud,  infixLed, extendDoubleCharacterOperator},
  ['!'] = 		  {70,  PREFIX, prefixNud, infixLed, extendDoubleCharacterOperator},
  ['='] = 		  { 5,  INFIX,  errorNud,  infixLed, extendDoubleCharacterOperator},
  ['~'] = 		  {70,  PREFIX, prefixNud, errorLed, extendSingleCharacterOperator},
  ['('] = 		  { 1,  PREFIX, prefixNud, infixLed, extendSingleCharacterOperator},
  [')'] = 		  { 0,  NOFIX,  prefixNud, infixLed, extendSingleCharacterOperator},
  ['['] = 		  { 1,  PREFIX, prefixNud, infixLed, extendSingleCharacterOperator},
  [']'] = 		  { 0,  NOFIX,  prefixNud, infixLed, extendSingleCharacterOperator},
  ['$'] = 		  { 0,  NOFIX,  errorNud,  infixLed, extendSingleCharacterOperator},
  ['#'] = 		  { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator},
  ['{'] = 		  { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator},
  ['}'] =		  { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator},
  ['@'] = 		  { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator},
  ['?'] =		  { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator},
  ['.'] = 		  { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator},
  [','] =		  { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator},
  [';'] =	 	  { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator},
  ['"'] =  		  { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator},
  ['\'']=		  { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator},
  ['`'] =		  { 1,  NOFIX,  errorNud,  errorLed, extendErrorOperator},
  ['\\']= 		  { 1,	NOFIX,  errorNud,  errorLed, extendErrorOperator},
  ['a' ... 'z'] = { 1,	NOFIX, 	errorNud,  errorLed, extendCharacterErrorOperator},
  ['A' ... 'Z'] = { 1,	NOFIX,	errorNud,  errorLed, extendCharacterErrorOperator},
  [48  ...  57] = { 1,	NOFIX,  errorNud,  errorLed, extendIntegerErrorOperator}
};

//this table defined the details of the token types. 
Attributes  tokenTypeAttributesTable[] = {
  [TOKEN_OPERATOR_TYPE]   = {0, NOFIX, prefixNud, infixLed, extendExpression},
  [TOKEN_UNKNOWN_TYPE]    = {0, NOFIX, errorNud,  errorLed, extendExpression},
  [TOKEN_INTEGER_TYPE]    = {0, NOFIX, expressionNud, errorLed, extendExpression},
  [TOKEN_FLOAT_TYPE]      = {0, NOFIX, expressionNud, errorLed, extendExpression},
  [TOKEN_STRING_TYPE]     = {0, NOFIX, expressionNud, errorLed, extendExpression},
  [TOKEN_IDENTIFIER_TYPE] = {0, NOFIX, expressionNud, errorLed, extendExpression}
};

/*Function of prefixNud is to grab the operator token.
 *	If the token is not operator, then return the token.
 *	
 */
Token* prefixNud(Token* myself){
  assert(myself != NULL);
  if(strcmp(((OperatorToken*)myself)->symbol,"(") != 0 \
  && strcmp(((OperatorToken*)myself)->symbol,"[") != 0)
    ((OperatorToken*)myself)->token[0] = parser(100);
  else
    ((OperatorToken*)myself)->token[0] = parser(0);
  
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
  ThrowError("This is an illegal prefix!", ERR_ILLEGAL_PREFIX);
  return myself;
}

// infixLed will assign INFIX or POSTFIX to the arity of OperatorToken.
Token* infixLed(Token* myself){
  assert(myself != NULL);
  if(strcmp(((OperatorToken*)myself)->symbol,"++") == 0 \
  || strcmp(((OperatorToken*)myself)->symbol,"--") == 0)
    ((OperatorToken*)myself)->arity = POSTFIX;
  else
    ((OperatorToken*)myself)->arity = INFIX;
  
  return myself;
}

/* if expression token passed into led,
 * 		errorLed will be called,
 *		errorCode: ERR_UNEXPECTED_EXPRESSION and errorMsg:"Expected operator token but obtained expression!"  will be throw.
 */
Token* errorLed(Token* myself){
  assert(myself != NULL);
  ThrowError("Expected operator token but obtained expression!", ERR_UNEXPECTED_EXPRESSION);
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
  }
  else
    ThrowError("This operator is undefined!", ERR_UNDEFINED_OPERATOR);  //Throw error such as "~X", where X can be any character
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
      ThrowError("This operator is undefined!", ERR_UNDEFINED_OPERATOR); //Throw error such as "*=X" detected, where X can be any character
  }
  else
    ThrowError("This operator is undefined!", ERR_UNDEFINED_OPERATOR); //Throw error when "*X" detected, where X can be any character
  
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
      ThrowError("This operator is undefined!", ERR_UNDEFINED_OPERATOR);      //Throw error when "+=X" detected, where X can be any character
  }
  else if(((OperatorToken*)thisOpr)->symbol[1] == ((OperatorToken*)thisOpr)->symbol[0]){
    if(((OperatorToken*)thisOpr)->symbol[2] == 0)
      ((OperatorToken*)thisOpr)->bindingPower = 70;
    else
      ThrowError("This operator is undefined!", ERR_UNDEFINED_OPERATOR);      //Throw error when "++X" detected, where X can be any character
  }
  else if(((OperatorToken*)thisOpr)->symbol[1] == 0)
    ((OperatorToken*)thisOpr)->bindingPower = attr->bindingPower;
  else
    ThrowError("This operator is undefined!", ERR_UNDEFINED_OPERATOR);        //Throw error when "+X" detected, where X can be any character other than '=' or itself
   
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
        ThrowError("This operator is undefined!", ERR_UNDEFINED_OPERATOR);        //Throw error when ">=X" detected, where X can be any character
    }
    else if(((OperatorToken*)thisOpr)->symbol[1] == ((OperatorToken*)thisOpr)->symbol[0]){
      if(((OperatorToken*)thisOpr)->symbol[2] == 0)
        ((OperatorToken*)thisOpr)->bindingPower = 40;
      else if(((OperatorToken*)thisOpr)->symbol[2] == '='){
        if(((OperatorToken*)thisOpr)->symbol[3] == 0)
          ((OperatorToken*)thisOpr)->bindingPower = 5;
        else
          ThrowError("This operator is undefined!", ERR_UNDEFINED_OPERATOR);      //Throw error when ">>=X" detected, where X can be any character
      }
      else
        ThrowError("This operator is undefined!", ERR_UNDEFINED_OPERATOR);        //Throw error when ">>X" detected, where X can be any character except '='
    }
    else
      ThrowError("This operator is undefined!", ERR_UNDEFINED_OPERATOR);          //Throw error when ">X" detected, where X can be any character except '=' and itself
    return thisOpr;
}


/* extendExpression is to differentiate the token type,
 *		if token is unknown type,
 *		errorCode: ERR_UNKNOWN_TOKEN and errorMsg:"Can't resolve Unknown type token!"  will be throw.
 *		else return the token.
 */
Token* extendExpression(Token *thisExpression, Attributes *attr){
  if(thisExpression->type == TOKEN_UNKNOWN_TYPE)
    ThrowError("Can't resolve Unknown type token!", ERR_UNKNOWN_TOKEN);
  else if(thisExpression->type == TOKEN_OPERATOR_TYPE){
    int thisSymbol  = *(char*)((OperatorToken*)thisExpression)->symbol;
    attr            = &operatorAttributesTable[thisSymbol];
    thisExpression  = attr->extend(thisExpression, attr);
  }
  else{
    thisExpression->nud = attr->nud;
    thisExpression->led = attr->led;
  }
  return thisExpression;
}

/*	If the operatorToken is a character,
 *	errorCode: ERR_ILLEGAL_CHARACTER and errorMsg:"Character operator is illegal!"  will be throw.
 */
Token* extendCharacterErrorOperator(Token *thisOpr, Attributes *attr){
  ThrowError("Character operator is illegal!", ERR_ILLEGAL_CHARACTER);
}

/*	If the operatorToken is a undefined operator,
 *	errorCode: ERR_UNDEFINED_OPERATOR and errorMsg:"This operator is undefined!"  will be throw.
 */
Token* extendErrorOperator(Token *thisOpr, Attributes *attr){
  ThrowError("This operator is undefined!", ERR_UNDEFINED_OPERATOR);
}

/*	If the operatorToken is an integer,
 *	errorCode: ERR_ILLEGAL_INTEGER and errorMsg:"Integer operator is illegal!"  will be throw.
 */
Token* extendIntegerErrorOperator(Token *thisOpr, Attributes *attr){
  ThrowError("Integer operator is illegal!", ERR_ILLEGAL_INTEGER);
}


