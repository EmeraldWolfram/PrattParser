#include "parser.h"
#include "Token.h"
#include "tokenExtend.h"
#include "CException.h"
#include "getToken.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern Attributes tokenTypeAttributesTable[];

/****************************************************************************************
 * 	This is the main function of the Pratt Parser.                                      *
 *  when an equation passed in,                                                         *
 *  the parser will check the equation from the left                                    *
 *  and link up a token tree according to the OPERATOR precedence.                      *
 *                                                                                      *
 *  example1: precedence of multiply(*) is higher than add(+),                          *
 *           so when an equation "2 + 3 * 4" entered to the parser,                     *
 *            it will link (3 * 4) and form a brunch,                                   *
 *            then parser will link the brunch to integer "2" by (+)                    *
 *            last, it will form a tree as shown below.                                 *
 *            This is an example of LINK DOWN.                                          *
 *                                                                                      *
 *                    (+)                                                               *
 *                    / \                                                               *
 *                  (2) (*)                                                             *
 *                      / \                                                             *
 *                    (3) (4)                                                           *
 *                                                                                      *                                                                                       *
 *  example2: precedence of sub(-) is smaller than multiply(*),                         *
 *           so when an equation "2 * 3 - 4" entered to the parser,                     *
 *            it will link (2 * 3) and form a brunch,                                   *
 *            then parser will link the brunch to integer "2" by (-)                    *
 *            last, it will form a tree as shown below.                                 *
 *            This is an example of LINK UP.                                            *
 *                                                                                      *
 *                    (-)                                                               *
 *                    / \                                                               *
 *                  (*) (4)                                                             *
 *                  / \                                                                 *
 *                (2) (3)                                                               *
 *                                                                                      *
 ****************************************************************************************/
Token* parser(int prevBindingPower){
 /***************************************************************************************
  *  These command allocated memory for the function to work.                           *
  ***************************************************************************************/
  Token* nextToken            = malloc(sizeof(Token));
  IntegerToken* nextIntToken  = malloc(sizeof(IntegerToken));
  OperatorToken* nextOprToken = malloc(sizeof(OperatorToken) + 2*(sizeof(Token*)));
  OperatorToken* currentToken = malloc(sizeof(OperatorToken) + 2*(sizeof(Token*)));
  Attributes* attr            = malloc(sizeof(Attributes));

 /***************************************************************************************
  *  These command assigned attributes to EXPRESSION for further usage in the function. *
  ***************************************************************************************/ 
  nextToken     = getToken();
  attr          = &tokenTypeAttributesTable[nextToken->type];
  nextIntToken  = (IntegerToken*)attr->extend(nextToken, attr);
  nextIntToken  = (IntegerToken*)nextToken->nud((Token*)nextIntToken);
  
 /***************************************************************************************
  *  These command assigned attributes to OPERATIONS for further usage in the function. *
  ***************************************************************************************/ 
  nextToken     = peepToken();
  attr          = &tokenTypeAttributesTable[nextToken->type];
  nextOprToken  = (OperatorToken*)attr->extend(nextToken, attr);
  nextOprToken  = (OperatorToken*)nextToken->led((Token*)nextOprToken);

 /***************************************************************************************
  *   When the precedence of the OPERATOR is larger than previous OPERATOR              *
  *   this command will link down to the next OPERATOR.                                 *
  ***************************************************************************************/
  if((nextOprToken->bindingPower)> prevBindingPower){
    nextOprToken = (OperatorToken*)getToken();
    nextOprToken->token[0] = (Token*)nextIntToken;
    if(nextOprToken->arity != POSTFIX)
      nextOprToken->token[1] = parser(nextOprToken->bindingPower);
  }
  else
    return (Token*)nextIntToken;
  
  
 /***************************************************************************************
  *   When the precedence of the OPERATOR is smaller than previous OPERATOR             *
  *   this command will link up to the next OPERATOR.                                   *
  ***************************************************************************************/  
  do{
    currentToken = (OperatorToken*)peepToken();
    attr         = &tokenTypeAttributesTable[currentToken->type];
    currentToken = (OperatorToken*)attr->extend((Token*)currentToken, attr);
    currentToken = (OperatorToken*)currentToken->led((Token*)currentToken);
    if(strcmp(currentToken->symbol, ")") == 0 \
    || strcmp(currentToken->symbol, "]") == 0){
      currentToken = (OperatorToken*)getToken();
      if(prevBindingPower == 1)
        return (Token*)nextOprToken;
    }
    if(currentToken->bindingPower > prevBindingPower){
      currentToken = (OperatorToken*)getToken();
      currentToken->token[0] = (Token*)nextOprToken;
      if(currentToken->arity != POSTFIX)
        currentToken->token[1] = parser(currentToken->bindingPower);
      nextOprToken = currentToken;
    }
  }while(strcmp(currentToken->symbol, "$") != 0 \
      && prevBindingPower == 0);
  return (Token*)nextOprToken;
}