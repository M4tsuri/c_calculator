#ifndef AST_H
#define AST_H
/**
 * generate an AST for given tokens
 * we use a LL(2) grammar here
 */

#include "project.h"

void gen_program(Project *proj);
void pretty_print_ast(StringTable *strtab, Expr *expr);

#endif