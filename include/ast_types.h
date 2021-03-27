#include "builtin.h"
#include "symtab.h"

/**
 * note that value type is different form variable type
 * value type works on both variable content and plain value
 * variable type only points out the type of varible symbol
 */
typedef enum VALUE_TYPE {
    VAL_INTEGER,
    VAL_FLOAT,
} VALUE_TYPE;

/**
 * operator of an unary expression 
 * unary_op = "-" | "+";
 */
typedef enum UNARY_OP_TYPE {
    // '+'
    UNARY_OP_POS,
    // '-'
    UNARY_OP_NEG,
} UNARY_OP_TYPE;

/**
 * operator of binary expression 
 * bin_op = "-" | "+" | "*" | "/";
 */
typedef enum BIN_OP_TYPE {
    // '+'
    BIN_OP_ADD,
    // '-'
    BIN_OP_SUB,
    // '/'
    BIN_OP_DIV,
    // '*'
    BIN_OP_MUL,
} BIN_OP_TYPE;

/* type of parenthesis */
typedef enum PARENTHESIS {
    // '('
    PARENTHESIS_OPEN,
    // ')'
    PARENTHESIS_CLOSE,
} PARENTHESIS;

/**
 * each expression returns a value
 * expression = unary_expr | bin_expr | parentheses_expr | value | symbol;
 */
typedef enum EXPR_TYPE {
    EXPR_UNARY,
    EXPR_BINARY,
    EXPR_PARENTHESES,
    EXPR_SYMBOL,
    EXPR_VALUE,
} EXPR_TYPE;

/**
 * statement = assignment | procedure_call;
 * be aware the statement do not return a value
 */
typedef enum STAT_TYPE {
    STAT_ASSIGN,
    STAT_CALL,
    STAT_DECL,
} STAT_TYPE;


/**
 * represents a plain value in program or a value stored in a variable
 */
typedef struct Value {
    VALUE_TYPE type;
    union {
        long long int int_val;
        long double float_val;
    } content;
} Value;

/**
 * unary_expr = unary_op, expression;
 */
typedef struct UnaryExpr {
    UNARY_OP_TYPE op;
    struct Expr *oprand;
} UnaryExpr;

/**
 * bin_expr = expression, bin_op, expression;
 */
typedef struct BinExpr {
    BIN_OP_TYPE op;
    struct Expr *lhs;
    struct Expr *rhs;
} BinExpr;

/**
 * just a wrapper of expression type to indicate it is enclosed in parentheses
 * parentheses_expr = "(", expression, ")";
 * TODO: determain whether to add a priority field here
 */
typedef struct ParenthesesExpr {
    struct Expr *content;
} ParenthesesExpr;

/**
 * an expression should return a value
 * 
 */
typedef struct Expr {
    EXPR_TYPE type;
    union {
        struct BinExpr bin_expr;
        struct UnaryExpr unary_expr;
        struct ParenthesesExpr paren_expr;
        struct Value value;
        size_t var_symid;
    } content;
} Expr;

/**
 * procedure_call = symbol, arg_list;
 * arg_list = ("(", ")") | "(", {expression, ","}, expression, ")";
 */
typedef struct ProcedureCall {
    BUILTIN_FUNC func;
    struct Expr arg;
} ProcedureCall;

/**
 * assign the value of src to dest
 */
typedef struct Assignment {
    int dest_idx;
    struct Expr *src;
} Assignment;

typedef struct Declaration {
    int sym_idx;
} Declaration;

/**
 * a statement is either an assignment or a call
 */
typedef struct Statement {
    STAT_TYPE type;
    union {
        struct Declaration decl;
        struct ProcedureCall call;
        struct Assignment assign;
    } content;
} Statement;

/**
 * program is a set of statement splited with ';' and end with '.'
 */
typedef struct Program {
    int stats_num;
    struct Statement *stats;
} Program;
