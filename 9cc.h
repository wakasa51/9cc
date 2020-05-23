#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Type Type;

// トークンの種類
typedef enum {
  TK_RESERVED, // 記号
  TK_IDENT,    // 識別子
  TK_NUM,      // 整数トークン
  TK_EOF,      // 入力の終わりを表すトークン
} TokenKind;

typedef struct Token Token;
struct Token {
  TokenKind kind; // トークンの型
  Token *next;    // 次の入力トークン
  int val;        // kindがTK_NUMの場合、その数値
  char *loc;      // トークン文字列
  int len;        // トークンの長さ
};

void error(char *fmt, ...);
void error_tok(Token *tok, char *fmt, ...);
bool equal(Token *tok, char *op);
Token *skip(Token *tok, char *op);
bool consume(Token **rest, Token *tok, char *str);
Token *tokenize(char *p);

typedef struct Var Var;
struct Var {
  Var *next;
  char *name;
  Type *ty;
  int offset;
};

typedef enum {
  ND_ADD,       // +
  ND_SUB,       // -
  ND_MUL,       // *
  ND_DIV,       // /
  ND_EQ,        // ==
  ND_NE,        // !=
  ND_LT,        // <
  ND_LE,        // <=
  ND_ASSIGN,    // =
  ND_ADDR,      // unary &
  ND_DEREF,     // unary *
  ND_RETURN,    // return
  ND_IF,        // if
  ND_FOR,       // for
  ND_BLOCK,     // {} (block)
  ND_FUNCALL,   // Function call
  ND_EXPR_STMT, // Expression statement
  ND_VAR,       // ローカル変数
  ND_NUM,       // Integer
} NodeKind;

typedef struct Node Node;
struct Node {
  NodeKind kind;
  Node *next;
  Type *ty;
  Token *tok;

  Node *lhs;
  Node *rhs;

  Node *cond;
  Node *then;
  Node *els;
  Node *init;
  Node *inc;

  Node *body;

  char *funcname;
  Node *args;

  Var *var; // Used if kind == ND_VAR
  long val; // Used if kind == ND_NUM
};

typedef struct Function Function;
struct Function {
  Function *next;
  char *name;
  Node *node;
  Var *locals;
  int stack_size;
};

Function *parse(Token *tok);

typedef enum {
  TY_INT,
  TY_PTR,
  TY_FUNC,
} TypeKind;

struct Type {
  TypeKind kind;

  Type *base;

  Token *name;

  Type *return_ty;
};

extern Type *ty_int;

bool is_integer(Type *ty);
Type *pointer_to(Type *base);
Type *func_type(Type *return_ty);
void add_type(Node *node);

void codegen(Function *prog);
