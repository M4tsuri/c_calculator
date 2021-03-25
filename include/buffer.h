#include <stdio.h>

// the maxinum length of a token
#define MAX_TOKEN_SIZE 63

typedef struct Buffer {
    // the length of buffer
    unsigned int len;
    // offset of the last unmatched non-empty charactor
    unsigned int cur;
    // current line number in source file 
    unsigned int cur_line;
    // the buffer containing the string to be processed
    char *src;
    void (*free)(struct Buffer *);
} Buffer;

Buffer *create_buffer(FILE *src);