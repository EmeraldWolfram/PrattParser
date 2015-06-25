#include "AddToken.h"
#include "Token.h"
#include <stdlib.h>
#include <stdio.h>

/**
 *  Add2Tokens help to generate a token tree by connecting two integer tokens
 *  with an operator token.
 *
 *  This function take 3 argument which is the operator symbol and two value
 *  to be stored in the two integer.
 *  
 *  Eg,
 *  Given:
 *         __________      _________       _________
 *        [ OPERATOR ]    [INTEGER 1]     [INTEGER 2]
 *        [  SYMBOL  ]    [  VALUE  ]     [  VALUE  ]
 *        [  ARITY   ]
 *        [*ptr][*ptr]
 *
 *  Should Return:
 *            __________
 *           [ OPERATOR ]
 *           [  SYMBOL  ]
 *           [  ARITY   ]
 *           [ptr*][ptr*]
 *   _________/         \_________
 *  [INTEGER 1]         [INTEGER 2]
 *  [  VALUE  ]         [  VALUE  ]
 *
 *  ptr in the diagram means pointer which store an address of a Token
 */




Token *add2Tokens(int leftValue, char *operatorSymbol, int rightValue){
  IntegerToken* leftTk = malloc(sizeof(IntegerToken));
  IntegerToken* rightTk = malloc(sizeof(IntegerToken));
  OperatorToken* addToken = malloc(sizeof(OperatorToken)+(sizeof(Token*))*2);

  addToken = createOperatorToken(operatorSymbol, INFIX);
  
  leftTk = createIntegerToken(leftValue);
  rightTk = createIntegerToken(rightValue);
  
  addToken->token[0] = (Token*)leftTk;
  addToken->token[1] = (Token*)rightTk;
  
  printf("Left TYPE: %d\t Value: %d\n",leftTk->type,leftTk->value);
  printf("Right TYPE: %d\t Value: %d\n",rightTk->type,rightTk->value);
  
  return (Token*)addToken;
}