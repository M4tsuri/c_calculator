#include "tokenize.h"
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "token_regex.h"

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

TOKEN_RES _match_pattern(const char *pattern, char *dest, Buffer *buf, bool cost) {
    regex_t regex;
    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        log_error("Internal error: regex compile failed.");
        exit(-1);
    }
    regmatch_t match;
    if (regexec(&regex, &buf->src[buf->cur], 1, &match, 0) == REG_NOMATCH) {
        return TOKEN_UNMATCH;
    }
    if (match.rm_so != 0) {
        return TOKEN_UNMATCH;
    }

    if (match.rm_eo > MAX_TOKEN_SIZE) {
        log_error("line %d: token length must less than %d.", buf->cur_line, MAX_TOKEN_SIZE);
        exit(-1);
    }

    strncpy(dest, &buf->src[buf->cur], match.rm_eo);

    if (cost) {
        buf->cur += match.rm_eo;
        ignore_empty(buf);
    }

    regfree(&regex);
    return TOKEN_MATCHED;
}

TOKEN_RES match_pattern(const char *pattern, char *dest, Buffer *buf) {
    return _match_pattern(pattern, dest, buf, true);
}

TOKEN_RES peek_pattern(const char *pattern, char *dest, Buffer *buf) {
    return _match_pattern(pattern, dest, buf, false);
}

/**
 * @return matched or not
 */
bool parse_symbol(StringTable *strtab, Buffer *buf, int *idx) {
    char symbol[MAX_TOKEN_SIZE + 1] = {0};
    if (match_pattern(REGEX_SYM, symbol, buf) != TOKEN_MATCHED) {
        return false;
    }

    size_t token_len = strlen(symbol);
    *idx = strtab_add(strtab, symbol, token_len);
    return true;
}

/**
 * expect a semicolon in buffer
 */ 
bool parse_semicolon(Buffer *buf) {
    if (token_unwrap(match_str(";", buf), buf->cur_line) != TOKEN_MATCHED) {
        return false;
    }
    return true;
}

/**
 * expect a eof signature
 */
bool parse_end(Buffer *buf) {
    if (token_unwrap(match_str(".", buf), buf->cur_line) != TOKEN_MATCHED) {
        return false;
    }
    return true;
}

bool parse_decl(Buffer *buf, DeclType *type) {
    char token_buf[MAX_TOKEN_SIZE];
    if (match_pattern(REGEX_DECL, token_buf, buf) != TOKEN_MATCHED) {
        return false;
    }
    if (token_buf[0] == 'i') {
        *type = DECL_INT;
    } else {
        *type = DECL_FLOAT;
    }
    return true;
}

void _tokenize(Pool *tokens, StringTable *strtab, Buffer *buf, TokenType state) {
    Token *t = pool_use(tokens);
    switch (state) {
        case TOKEN_DECL:
            if (parse_symbol(strtab, buf, &t->content.name_idx)) {
                t->type = TOKEN_DSYMBOL;
            } else {
                panic(buf->cur_line, "expected symbol after declaration keyword.\n");
            }
            break;
        case TOKEN_DSYMBOL:
            if (parse_semicolon(buf)) {
                t->type = TOKEN_SEMICOLON;
            } else if (parse_end(buf)) {
                t->type = TOKEN_END;
            } else {
                panic(buf->cur_line, "expected a semicolon or eof after declaration.\n");
            }
            break;
        case TOKEN_SEMICOLON:
            if (parse_decl(buf, &t->content.decl)) {
                t->type = TOKEN_DECL;
            } else if (parse_symbol(strtab, buf, &t->content.name_idx)) {
                t->type = TOKEN_LSYMBOL;
            } else {
                panic(buf->cur_line, "expected a declaration or assignment.\n");
            }
            break;
        case TOKEN_END:
            return;
        default:
            panic(buf->cur_line, "corrupted source code.");
    }
    _tokenize(tokens, strtab, buf, t->type);
}

// entrance of the automaton for parsing tokens
void tokenize(Pool *token_pool, StringTable *strtab, Buffer *buf) {
    Token *t = pool_use(token_pool);
    char first_decl[8];
    if (match_pattern(REGEX_DECL, first_decl, buf) != TOKEN_MATCHED) {
        log_error("the program must start with a declaration.");
        exit(-1);
    }

    t->type = TOKEN_DECL;
    if (first_decl[0] == 'i') {
        t->content.decl = DECL_INT;
    } else {
        t->content.decl = DECL_FLOAT;
    }

    _tokenize(token_pool, strtab, buf, t->type);
}

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