#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include "tokenize.h"

int main(int argc, char **argv) {
    
    Project *proj = create_project(argc, argv);
    tokenize(proj);

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
                printf("semicolon ");
                break;
            case TOKEN_END:
                printf("end ");
                break;
            case TOKEN_LSYMBOL:
                printf("lsymbol(%s) ", strtab_get(proj->strtab, t->content.name_idx));
                break;
            case TOKEN_NUMBER:
                printf("number(%Lf) ", t->content.number);
                break;
            case TOKEN_ASSIGN:
                printf("assign ");
                break;
            default:
                printf("none ");
                break;
        }
    }
    // release all sources
    FREE(proj);
    return 0;
}