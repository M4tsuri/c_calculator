#include "buffer.h"
#include <stdlib.h>

void delete_buffer(Buffer *buf) {
    free(buf->src);
    free(buf);
}

Buffer *create_buffer(FILE *src) {
    Buffer *buf = (Buffer *) malloc(sizeof(Buffer));

    // get the length of original source code
    fseek(src, 0, SEEK_END);
    buf->len = ftell(src);
    rewind(src);

    // read source code to buffer
    buf->src = (char *) malloc(buf->len);
    fread(buf->src, buf->len, 1, src);
    buf->cur = 0;
    buf->cur_line = 1;
    buf->free = delete_buffer;
    return buf;
}