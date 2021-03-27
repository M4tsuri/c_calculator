#include "symtab.h"
#include <stdbool.h>

void delete_symtab(SymbolTable *symtab) {
    free(symtab->symbols);
    free(symtab);
}

SymbolTable *create_symtab() {
    SymbolTable *symtab = (SymbolTable *) s_malloc(sizeof(SymbolTable));
    symtab->capacity = DEFAULT_MAX_SYMTAB_ITEM_NUM;
    symtab->symbols = (Symbol *) s_malloc(symtab->capacity * sizeof(Symbol));
    symtab->sym_num = 0;
    symtab->max_func_tid = 0;
    symtab->max_var_tid = 0;
    symtab->free = delete_symtab;
    return symtab;
}

int symtab_find(SymbolTable *symtab, unsigned int name_idx) {
    for (int i = 0; i < symtab->sym_num; ++i) {
        if (symtab->symbols[i].name_idx == name_idx) {
            return i;
        }
    }
    return -1;
}

int symtab_push(SymbolTable *symtab, Symbol *s) {
    if (symtab_find(symtab, s->name_idx)) {
        return -1;
    }

    if (symtab->sym_num >= symtab->capacity) {
        symtab->capacity *= 2;
        symtab->symbols = (Symbol *) s_realloc(symtab->symbols, symtab->capacity * sizeof(Symbol));
    }
    memcpy(&symtab->symbols[symtab->sym_num], s, sizeof(Symbol));
    symtab->sym_num++;
    if (s->type == SYM_FUNCTION) {
        symtab->max_func_tid++;
    } else {
        symtab->max_var_tid++;
    }

    return symtab->sym_num - 1;
}

Symbol *symtab_get(SymbolTable *symtab, size_t idx) {
    if (idx >= symtab->sym_num) {
        panic(0, "internal error: symbol table out of bound access.");
    }
    return &symtab->symbols[idx];
}

