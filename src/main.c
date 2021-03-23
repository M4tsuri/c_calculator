#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "parseopt.h"
#include <sys/stat.h>
#include "tokenize.h"

int main(int argc, char **argv) {
    FILE *src = get_src(argc, argv);
    FILE *dest = get_dest(argc, argv);
    
    // get the length of original source code
    fseek(src, 0, SEEK_END);
    size_t src_length = ftell(src);
    rewind(src);

    // read source code to buffer
    char *src_buf = (char *) malloc(src_length);
    fread(src_buf, src_length, 1, src);

    // split source code into tokens
    Pool *tokens = tokenize(src_buf, src_length);

    
    free(src_buf);
    return 0;
}