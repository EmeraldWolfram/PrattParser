#ifndef Token_H
#define Token_H
#define MUL 30
#define DIV 30
#define SUB 20
#define ADD 20
#define EOT 0

typedef enum {
  TOKEN_INTEGER_TYPE,
  TOKEN_OPERATOR_TYPE,
}TokenType;

typedef enum {
  PREFIX,
  INFIX,
  POSTFIX,
} Arity;

typedef struct {
  TokenType type;
}Token;

typedef struct{
  TokenType type;
  int value;
}IntegerToken;

typedef struct{
  TokenType type;
  char *symbol;
  Arity arity;
  int bindingPower;
  Token *token[0];
} OperatorToken;

OperatorToken *createOperatorToken(char *symbol, Arity arity);
IntegerToken *createIntegerToken(int value);

#endif // Token_H
