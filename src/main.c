#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include "tokenize.h"
#include "ast.h"
#include "execute.h"


int main(int argc, char **argv) {
    
    Project *proj = create_project(argc, argv);
    tokenize(proj);

    log_success("Tokenize finished.\n");
#ifdef DEBUG
    Token *t;
    FOR_EACH(t, proj->tokens) {
        switch (t->type) {
            case TOKEN_DECL:
                printf("decl ");
                break;
            case TOKEN_DSYMBOL:
                printf("dsymbol(%s) ", strtab_get(proj->strtab, t->content.name_idx));
                break;
            case TOKEN_SEMICOLON:
                printf(";\n");
                break;
            case TOKEN_END:
                printf(". ");
                break;
            case TOKEN_LSYMBOL:
                printf("lsymbol(%s) ", strtab_get(proj->strtab, t->content.name_idx));
                break;
            case TOKEN_NUMBER:
                printf("number(%Lf) ", t->content.number);
                break;
            case TOKEN_ASSIGN:
                printf("= ");
                break;
            case TOKEN_RSYMBOL:
                printf("rsymbol(%s) ", strtab_get(proj->strtab, t->content.name_idx));
                break;
            case TOKEN_UNOP:
                printf("unop ");
                break;
            case TOKEN_BINOP:
                printf("binop ");
                break;
            case TOKEN_LPAREN:
                printf("(%ld ", t->content.rparen_idx);
                break;
            case TOKEN_RPAREN:
                printf("%ld) ", pool_idx(proj->tokens, t));
                break;
            default:
                printf("none ");
                break;
        }
    }
    reset_iter(proj->tokens);
    printf("\n");
#endif

    gen_program(proj);

    Statement *s;
    log_success("AST generation finished.\n");

#ifdef DEBUG
    FOR_EACH(s, proj->program->statements) {
        if (s->type == STAT_ASSIGN) {
            pretty_print_ast(proj, s->content.assign.src);
            printf("\n");
        }
    }
    reset_iter(proj->program->statements);
#endif

    exec_ast(proj);
    log_success("AST execution finished, result saved.\n");

    // release all sources
    FREE(proj);
    return 0;
}