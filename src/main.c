#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include "tokenize.h"

int main(int argc, char **argv) {
    FILE *src = get_src(argc, argv);
    FILE *dest = get_dest(argc, argv);

    // split source code into tokens
    Buffer *buf = create_buffer(src);
    Pool *tokens = create_pool(sizeof(Token));
    StringTable *strtab = create_strtab();
    tokenize(tokens, strtab, buf);

    Token *t;
    FOR_EACH(t, tokens) {
        switch (t->type) {
            case TOKEN_DECL:
                printf("decl ");
                break;
            case TOKEN_DSYMBOL:
                printf("dsymbol(%s) ", strtab_get(strtab, t->content.name_idx));
                break;
            case TOKEN_SEMICOLON:
                printf("semicolon ");
                break;
            case TOKEN_END:
                printf("end");
                break;
            default:
                printf("none ");
                break;
        }
    }
    // release all sources
    FREE(buf);
    FREE(tokens);
    FREE(strtab);
    
    return 0;
}