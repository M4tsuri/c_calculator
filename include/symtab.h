#include "pool.h"
#include "utils.h"
#define DEFAULT_MAX_SYMTAB_ITEM_NUM 0x40

typedef unsigned int TidType;

/**
 * a symbol can be either a function or a variable containing a plain value
 */
typedef enum SYMBOL_TYPE {
    SYM_VARIABLE_TYPE,
    SYM_FUNCTION,
} SYMBOL_TYPE;

/**
 * a symbol is an abstract of variables defined in program
 * each symbol has a tid to symplify program analysis
 */
typedef struct Symbol {
    SYMBOL_TYPE type;
    unsigned int name_idx;
} Symbol;

typedef struct SymbolTable {
    int capacity;
    int max_func_tid;
    int max_var_tid;
    int sym_num;
    struct Symbol *symbols;
    void (*free)(struct SymbolTable *);
} SymbolTable;

SymbolTable *create_symtab();

/**
 * insert a symbol to symbol table
 * @param s: the symbol we want to instert, the content will be deep copied into table
 * @return the index of this symbol in symbol table, -1 if this symbol already exists in table
 */
int symtab_push(SymbolTable *symtab, Symbol *s);

/**
 * get a symbol by its index in symbol table
 */
Symbol *symtab_get(SymbolTable *symtab, size_t idx);

/**
 * find a symbol in symbol table
 * @param name_idx: the index of the symbol name in string table
 * @return the index of the symbol, -1 if symbol not in symtab
 */
int symtab_find(SymbolTable *symtab, unsigned int name_idx);
