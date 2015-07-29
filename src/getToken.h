#ifndef getToken_H
#define getToken_H
#include "Token.h"
#include "Common.h"

//FOR MOCKING PURPOSE
INLINE Token *getToken();  //get the detail of token and move the pointer to next token 
INLINE Token *peepToken(); //get the detail of token and remain the pointer to current token

#endif // getToken_H
