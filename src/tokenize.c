#include "tokenize.h"
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "log.h"

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
    return TOKEN_UNMATCH;
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

TOKEN_RES match_pattern(const char *pattern, char *dest, Buffer *buf) {
    return TOKEN_UNMATCH;
}

TOKEN_RES peek_pattern(const char *pattern, Buffer *buf) {
    regex_t regex;
    if (regcomp(&regex, pattern, 0) != 0) {
        log_error("Internal error: regex compile failed.");
        exit(-1);
    }
    regmatch_t match;
    regexec(&regex, &buf->src[buf->cur], 1, &match, 0);
    if (match.rm_so != 0) {
        log_error("internal error: invalid usage of peek_pattern, can only match from begining of line.");
        exit(-1);
    }
}

Pool *tokenize(char *src, size_t len) {
    return NULL;
}