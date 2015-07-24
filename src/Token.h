#ifndef Token_H
#define Token_H

#include <stdint.h>

#define EOT 0

#define Octal 8
#define Decimal 10
#define Hexdecimal 16

typedef enum {
	TOKEN_UNKNOWN_TYPE,
	TOKEN_INTEGER_TYPE,
	TOKEN_FLOAT_TYPE,
	TOKEN_OPERATOR_TYPE,
	TOKEN_STRING_TYPE,
	TOKEN_IDENTIFIER_TYPE,
} TokenType;

typedef enum {
	PREFIX, //-2
	INFIX,	//2*2
	POSTFIX,	//x++
  NOFIX
} Arity;

typedef enum {
	NONE,
	LEFT_TO_RIGHT,
	RIGHT_TO_LEFT,
} Associativity;

typedef struct Token_t Token;
struct Token_t{
	TokenType type;
  uint32_t startColumn;
  uint32_t length;
  Token* (*nud)(Token*);
  Token* (*led)(Token*);
  char* str;
};

typedef struct {
	TokenType type;
  uint32_t startColumn;
  uint32_t length;
  Token* (*nud)(Token*);
  Token* (*led)(Token*);
  char* str;
  int value;
} IntegerToken;

typedef struct {
	TokenType type;
  uint32_t startColumn;
  uint32_t length;
  Token* (*nud)(Token*);
  Token* (*led)(Token*);
  char* str;
  double value;
} FloatToken;

typedef struct {
	TokenType type;
  uint32_t startColumn;
  uint32_t length;
  Token* (*nud)(Token*);
  Token* (*led)(Token*);
  char* str;
  char *name;
	Token* token;
} IdentifierToken, StringToken;

typedef struct {
	TokenType type;
  uint32_t startColumn;
  uint32_t length;
  Token* (*nud)(Token*);
  Token* (*led)(Token*);
  char* str;
  char *symbol;
  int bindingPower;
  Arity arity;
  Token *token[0];
} OperatorToken;

Token *createOperatorToken(char *symbol, Arity AR);
Token *createIntegerToken(int value);



#endif // Token_H

