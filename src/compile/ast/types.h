#include "builtin.h"

/* tid is a unique for assigned to each item in a program */
typedef unsigned int TidType;

/* we support integer and float number now */
typedef enum VARIABLE_TYPE {
    VAR_INTEGER,
    VAR_FLOAT,
} VARIABLE_TYPE;

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
} STAT_TYPE;

/**
 * a symbol can be either a function or a variable containing a plain value
 */
typedef enum SYMBOL_TYPE {
    SYM_VARIABLE_TYPE,
    SYM_FUNCTION,
} SYMBOL_TYPE;

/**
 * we only support builtin functions now 
 */
typedef enum FUNC_TYPE {
    FUNC_BUILTIN,
    FUNC_CUSTOM,
    FUNC_EXTERNAL,
} FUNC_TYPE;

/**
 * function is a type of symbol, 
 * we use prototype to identify a function
 * TODO: we need a target field for non-builtin functions
 */
typedef struct Function {
    TidType tid;
    FUNC_TYPE type;
    Symbol *symbol;
    FunctionPrototype *prototype;
    /* 
     * we realize builtin function inside our runtime engine
     * now we only support this type of function 
     */
    int is_builtin;

};

/**
 * a symbol is an abstract of variables defined in program
 * each symbol has a tid to symplify program analysis
 */
typedef struct Symbol {
    TidType tid;
    SYMBOL_TYPE type;
    union content {
        Variable *variable;
        Function *function;
    };
} Symbol;

/**
 * represents a plain value in program or a value stored in a variable
 */
typedef struct Value {
    VALUE_TYPE type;
    union content {
        long long int int_val;
        long double float_val;
    };
} Value;

/* a varibale represents either an integer or a float number */
typedef struct Variable {
    TidType tid;
    VARIABLE_TYPE type;
    char *name;
    Value *value;
} Variable;

/* we use function prototype to uniquely identify a function */
typedef struct FunctionPrototype {
    char *name;
    SYMBOL_TYPE* args;
} FunctionPrototype;

/**
 * unary_expr = unary_op, expression;
 */
typedef struct UnaryExpr {
    UNARY_OP_TYPE op;
    Expr *oprand;
} UnaryExpr;

/**
 * bin_expr = expression, bin_op, expression;
 */
typedef struct BinExpr {
    BIN_OP_TYPE op;
    Expr *lhs;
    Expr *rhs;
} BinExpr;

/**
 * just a wrapper of expression type to indicate it is enclosed in parentheses
 * parentheses_expr = "(", expression, ")";
 * TODO: determain whether to add a priority field here
 */
typedef struct ParenthesesExpr {
    Expr *content;
} ParenthesesExpr;

/**
 * an expression should return a value
 * 
 */
typedef struct Expr {
    EXPR_TYPE type;
    union content {
        BinExpr *bin_expr;
        UnaryExpr *unary_expr;
        ParenthesesExpr *paren_expr;
        Value *value;
        Variable *variable;
    };
} Expr;

/**
 * procedure_call = symbol, arg_list;
 * arg_list = ("(", ")") | "(", {expression, ","}, expression, ")";
 */
typedef struct ProcedureCall {
    Function* target;
    Expr *args[];
} ProcedureCall;

/**
 * assign the value of src to dest
 */
typedef struct Assignment {
    Variable* dest;
    Expr *src;
} Assignment;

/**
 * a statement is either an assignment or a call
 */
typedef struct Statement {
    TidType tid;
    STAT_TYPE type;
    union content {
        ProcedureCall *call;
        Assignment *assign;
    };
} Statement;

/**
 * program is a set of statement splited with ';' and end with '.'
 */
typedef struct Program {
    TidType tid;
    Statement *stats[];
} Program;
