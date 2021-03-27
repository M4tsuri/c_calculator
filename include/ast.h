#include "project.h"

void generate_program(Project *proj) {
    // get statement number
    Token *t;
    FOR_EACH(t, proj->tokens) {
        if (t->type == TOKEN_SEMICOLON) {
            proj->program->stats_num++;
        }
    }
    reset_iter(proj->tokens);
    proj->program->stats_num++;

    proj->program->stats = (Statement *) s_malloc(sizeof(Statement));
    generate_statements(proj);
}

void generate_parenexpr(Project *proj, size_t stat_idx) {

}

void generate_unexpr(Project *proj, size_t stat_idx) {

}

void generate_binexpr(Project *proj, size_t stat_idx) {

}


void generate_expression(Project *proj, size_t stat_idx) {
    Token *t = pool_peek(proj->tokens);
    Statement *cur_stat = &proj->program->stats[stat_idx];

    switch (t->type) {
        case TOKEN_LPAREN:
            generate_parenexpr(proj, stat_idx);
            break;
        case TOKEN_UNOP:
            generate_unexpr(proj, stat_idx);
            break;
        case TOKEN_NUMBER:
        case TOKEN_RSYMBOL:
            return;
    }
}

void _generate_statement(Project *proj, size_t stat_idx) {
    Token *t = pool_next(proj->tokens);
    Token *next = pool_next(proj->tokens);
    Symbol sym;
    Statement *cur_stat = &proj->program->stats[stat_idx];

    if (t->type == TOKEN_DECL) {
        // add this declared symbol to symbol table
        sym.type = SYM_VARIABLE_TYPE;
        // get the symbol name from the next token content
        sym.name_idx = next->content.name_idx;
        sym.content.variable.tid = proj->symtab->max_var_tid;
        if (t->content.decl == DECL_FLOAT) {
            sym.content.variable.type = VAR_FLOAT;
        } else if (t->content.decl == DECL_INT) {
            sym.content.variable.type = VAR_INTEGER;
        } else {
            panic(t->line, "initernal error: error when dealing with declaration type.");
        }

        int sym_idx = symtab_push(proj->symtab, &sym);
        if (sym_idx == -1) {
            panic(t->line, "duplicated declaration of variable.");
        }

        // initialize this statement
        cur_stat->content.decl.sym_idx = sym_idx;

    } else if (t->type == TOKEN_LSYMBOL) {
        // parse an assignment
        if (next->type == TOKEN_ASSIGN) {
            cur_stat->type = STAT_ASSIGN;
            // find the assigned value in symtab, if not found, exit with error msg
            int dest_idx = symtab_find(proj->symtab, t->content.name_idx);
            if (dest_idx == -1) {
                panic(t->line, "value used without declaration.");
            }
            cur_stat->content.assign.dest_idx = dest_idx;

            // parsing the rhs of the assignment
            generate_expression(proj, stat_idx);
        } else if (next->type == TOKEN_LPAREN) {
            cur_stat->type = STAT_CALL;
            if (strcmp("write", strtab_get(proj->strtab, t->content.name_idx))) {
                cur_stat->content.call.func = BUILTIN_WRITE;
            }
            generate_expression(proj, stat_idx);

            // we expect a ')' after arg
            Token *rparen = pool_next(proj->tokens);
            if (rparen->type != TOKEN_RPAREN) {
                panic(rparen->line, "internal error: expression parsing failed.");
            }
        }
    }

    Token *eol = pool_next(proj->tokens);
    if (eol->type == TOKEN_END) {
        return;
    } else if (eol->type == TOKEN_SEMICOLON) {

    }

    _generate_statement(proj, stat_idx + 1);
}

void generate_statements(Project *proj) {
    _generate_statement(proj, 0);
}
