#ifndef execute_H
#define execute_H
#include "Token.h"

char* executeSingle(Token* token);
char* executeDouble(Token* token);
char* executeExpression(Token* token);

#endif // execute_H
