typedef enum DeclType {
    DECL_INT,
    DECL_FLOAT,
} DeclType;

typedef enum BinOpType {
    BINOP_ADD,
    BINOP_SUB,
    BINOP_MUL,
    BINOP_DIV,
} BinOpType;

typedef enum UnOpType {
    UNOP_POS,
    UNOP_NEG
} UnOpType;

/**
 * for a program
 * 
 * float a; int b;
 * a = b + 1;
 * write(a).
 * 
 * it can be tokenized into
 * 
 * TOKEN_DECL(DECL_FLOAT) TOKEN_SYMBOL('a') TOKEN_SEMICOLON TOKEN_DECL(DECL_INT) TOKEN_SYMBOL('b')
 * TOKEN_SEMICOLON TOKEN_SYMBOL('a') TOKEN_OP(OP_ASSIGN) TOKEN_SYMBOL('b') TOKEN_OP(OP_ADD)
 * TOKEN_NUMBER(1.0) TOKEN_SEMICOLON TOKEN_SYMBOL("write") TOKEN_OPEN_PAREN TOKEN_SYMBOL('a')
 * TOKEN_CLOSE_PAREM TOKEN_END
 */
typedef enum TokenType {
    TOKEN_NUMBER,
    TOKEN_DSYMBOL,       // including variable name and function name
    TOKEN_RSYMBOL,
    TOKEN_LSYMBOL,
    TOKEN_UNOP,
    TOKEN_ASSIGN,
    TOKEN_BINOP,
    TOKEN_END,
    TOKEN_SEMICOLON,
    TOKEN_DECL,
    TOKEN_OPEN_PAREN,
    TOKEN_CLOSE_PAREN,
} TokenType;

typedef struct Token {
    TokenType type;
    union {
        DeclType decl;
        long double number;
        UnOpType un_op;
        BinOpType bin_op;
        // the index of name in strtab
        unsigned int name_idx;
    } content;
} Token;
