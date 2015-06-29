#ifndef Token_H
#define Token_H

#include <stdint.h>

typedef enum {
	TOKEN_UNKNOWN_TYPE,
	TOKEN_INTEGER_TYPE,
	TOKEN_OPERATOR_TYPE,
	TOKEN_FLOAT_TYPE,
	TOKEN_STRING_TYPE,
	TOKEN_IDENTIFIER_TYPE,
} TokenType;

typedef enum {
	PREFIX, //-2
	INFIX,	//2*2
	POSTFIX	//x++
} Arity;

typedef enum {
	NONE,
	LEFT_TO_RIGHT,
	RIGHT_TO_LEFT,
} Associativity;

typedef struct {
	TokenType type;
  uint32_t startColumn;
  uint32_t length;
} Token;

typedef struct {
	TokenType type;
  uint32_t startColumn;
  uint32_t length;
	int value;
} IntegerToken;

typedef struct {
	TokenType type;
  uint32_t startColumn;
  uint32_t length;
	double value;
} FloatToken;

typedef struct {
	TokenType type;
  uint32_t startColumn;
  uint32_t length;
	char *name;
} IdentifierToken, StringToken;

typedef struct {
	TokenType type;
  uint32_t startColumn;
  uint32_t length;
	char *symbol;
	Arity arity;
	Token *token[0];
} OperatorToken;

Token *createOperatorToken(char *symbol, Arity AR);
Token *createIntegerToken(int value);

#endif // Token_H

