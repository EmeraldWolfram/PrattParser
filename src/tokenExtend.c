#include "tokenExtend.h"
#include "Token.h"
#include "ErrorObject.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Token* extendSingleCharacterOperator(Token* thisOpr, OperatorAttributes *attr){
  ((OperatorToken*)thisOpr)->bindingPower  = attr->bindingPower;
  ((OperatorToken*)thisOpr)->arity         = attr->arity;
  ((OperatorToken*)thisOpr)->nud           = attr->nud;
  ((OperatorToken*)thisOpr)->led           = attr->led;
  
  return thisOpr;
}

Token* extendDoubleCharacterOperator(Token *thisOpr, OperatorAttributes *attr){ 
  ((OperatorToken*)thisOpr)->nud           = attr->nud;
  ((OperatorToken*)thisOpr)->led           = attr->led;
  
  if(((OperatorToken*)thisOpr)->symbol[1] == '='){
    ((OperatorToken*)thisOpr)->bindingPower  = 5;
    ((OperatorToken*)thisOpr)->arity         = INFIX;
  }
  else if(((OperatorToken*)thisOpr)->symbol[1] == 0){
    ((OperatorToken*)thisOpr)->bindingPower  = attr->bindingPower;
    ((OperatorToken*)thisOpr)->arity         = attr->arity;
  }
  else
    printf("ERROR"); //ThrowError
  
  if(strcmp(((OperatorToken*)thisOpr)->symbol, "==") == 0)
    ((OperatorToken*)thisOpr)->bindingPower = 20;
  
  
  return thisOpr;
}

Token* extendTripleCharacterOperator(Token *thisOpr, OperatorAttributes *attr){
  ((OperatorToken*)thisOpr)->nud           = attr->nud;
  ((OperatorToken*)thisOpr)->led           = attr->led;
  ((OperatorToken*)thisOpr)->arity        = attr->arity;
  
  if(((OperatorToken*)thisOpr)->symbol[1] == '='){
    ((OperatorToken*)thisOpr)->bindingPower = 5;
    ((OperatorToken*)thisOpr)->arity        = INFIX;
  }
  else if(((OperatorToken*)thisOpr)->symbol[1] == ((OperatorToken*)thisOpr)->symbol[0])
    ((OperatorToken*)thisOpr)->bindingPower = 70;
  else if(((OperatorToken*)thisOpr)->symbol[1] == 0)
    ((OperatorToken*)thisOpr)->bindingPower = attr->bindingPower;
  else
    printf("ERROR");//ThrowError
  
  if(strcmp(((OperatorToken*)thisOpr)->symbol, "&&") == 0){
    ((OperatorToken*)thisOpr)->bindingPower = 7;
    ((OperatorToken*)thisOpr)->arity         = INFIX;
    }
  if(strcmp(((OperatorToken*)thisOpr)->symbol, "||") == 0){
    ((OperatorToken*)thisOpr)->bindingPower = 6;
  }
  
  return thisOpr;
}

Token* extendQuadrupleCharacterOperator(Token *thisOpr, OperatorAttributes *attr){
  ((OperatorToken*)thisOpr)->arity         = attr->arity;
  ((OperatorToken*)thisOpr)->nud           = attr->nud;
  ((OperatorToken*)thisOpr)->led           = attr->led;
  
  if(((OperatorToken*)thisOpr)->symbol[1] == 0)
    ((OperatorToken*)thisOpr)->bindingPower = attr->bindingPower;  
  else if(((OperatorToken*)thisOpr)->symbol[1] == '=')
    ((OperatorToken*)thisOpr)->bindingPower = attr->bindingPower;
  else if(((OperatorToken*)thisOpr)->symbol[1] == ((OperatorToken*)thisOpr)->symbol[0]){
    if(((OperatorToken*)thisOpr)->symbol[2] == 0)
      ((OperatorToken*)thisOpr)->bindingPower = 40;
    else if(((OperatorToken*)thisOpr)->symbol[2] == '=')
      ((OperatorToken*)thisOpr)->bindingPower = 5;
    else
      printf("ERROR"); //ThrowError
  }
  else
    printf("ERROR");//THROW ERROR
  
}
