#include "pool.h"
#define DEFAULT_MAX_SYMTAB_ITEM_NUM 0x40

typedef unsigned int TidType;

/* we support integer and float number now */
typedef enum VARIABLE_TYPE {
    VAR_INTEGER,
    VAR_FLOAT,
} VARIABLE_TYPE;

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
typedef struct FunctionSym {
    TidType tid;
    FUNC_TYPE type;
    struct FunctionPrototype *prototype;
    /* 
     * we realize builtin function inside our runtime engine
     * now we only support this type of function 
     */
    int is_builtin;

} FunctionSym;

/**
 * a symbol is an abstract of variables defined in program
 * each symbol has a tid to symplify program analysis
 */
typedef struct Symbol {
    SYMBOL_TYPE type;
    union {
        struct VariableSym *variable;
        struct FunctionSym *function;
    } content;
} Symbol;

/* a variable represents either an integer or a float number */
typedef struct VariableSym {
    TidType tid;
    VARIABLE_TYPE type;
    char *name;
    struct Value *value;
} VariableSym;

/* we use function prototype to uniquely identify a function */
typedef struct FunctionPrototype {
    char *name;
    SYMBOL_TYPE* args;
} FunctionPrototype;

typedef struct SymbolTable {
    size_t capacity;
    size_t max_func_tid;
    size_t max_var_tid;
    size_t sym_num;
    struct Symbol *symbols;
    void (*free)(struct SymbolTable *);
} SymbolTable;

SymbolTable *create_symtab();

size_t symtab_push(SymbolTable *symtab, Symbol *s);

Symbol *symtab_get(SymbolTable *symtab, size_t idx);
