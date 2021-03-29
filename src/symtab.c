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

void symtab_assign(SymbolTable *symtab, int sym_idx, Value *val) {
    Symbol *s = symtab_get(symtab, sym_idx);
    switch (s->type) {
        case DECL_FLOAT:
            if (val->type == VAL_INTEGER) {
                s->content.value.content.float_val = (long double)val->content.int_val;
                s->content.value.type = VAL_FLOAT;
            } else {
                s->content.value = *val;
            }
            break;
        case DECL_INT:
            if (val->type == VAL_FLOAT) {
                s->content.value.content.int_val = (long long int)val->content.float_val;
                s->content.value.type = VAL_INTEGER;
            } else {
                s->content.value = *val;
            }
            break;
        default:
            panic(0, "not implemented.");
    }
}

int symtab_push(SymbolTable *symtab, Symbol *s) {
    if (symtab_find(symtab, s->name_idx) != -1) {
        return -1;
    }

    if (symtab->sym_num >= symtab->capacity) {
        symtab->capacity *= 2;
        symtab->symbols = (Symbol *) s_realloc(symtab->symbols, symtab->capacity * sizeof(Symbol));
    }
    memcpy(&symtab->symbols[symtab->sym_num], s, sizeof(Symbol));
    symtab->sym_num++;

    return symtab->sym_num - 1;
}

Symbol *symtab_get(SymbolTable *symtab, size_t idx) {
    if (idx >= symtab->sym_num) {
        panic(0, "internal error: symbol table out of bound access.");
    }
    return &symtab->symbols[idx];
}

