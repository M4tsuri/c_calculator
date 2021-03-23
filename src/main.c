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


    // release all sources
    FREE(buf);
    FREE(tokens);
    FREE(strtab);
    
    return 0;
}