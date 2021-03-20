#include "tokenize.h"
#include "../res_handle/res_handle.h"
#include <stdlib.h>
#include <string.h>

/**
 * move src->cur to the index of next non-empty charactor and update src->cur_line correspondingly
 * note that space, tab and newline are all empty charactors
 */
void ignore_empty(Buffer *src) {
    for ( ; ; src->cur++) {
        switch (src->src[src->cur]) {
            case '\n':
                src->cur_line++;
                break;
            case ' ':
            case '\t':
                break;
            default:
                return;
        }
    }
}

TOKEN_RES match_str(char *str, Buffer *src) {
    unsigned str_len = strlen(str);
    if (token_unwrap(peek_str(str, src), src->cur_line) == TOKEN_UNMATCH) {
        return TOKEN_UNMATCH;
    }
    src->cur += str_len;
    ignore_empty(src);
    return TOKEN_MATCHED;
}

TOKEN_RES match_num(long double *res, Buffer *src) {

}

TOKEN_RES peek_str(char *str, Buffer *src) {
    unsigned str_len = strlen(str);
    if (str_len > src->len - src->cur) {
        return TOKEN_UNEXPECTED_END;
    }

    if (strncmp(str, &src->src[src->cur], str_len) == 0) {
        return TOKEN_MATCHED;
    }
    return TOKEN_UNMATCH;
}

TOKEN_RES peek_num(Buffer *src) {
    
}
