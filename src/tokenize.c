#include "tokenize.h"
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "token_regex.h"

/**
 * an Automation Node is a Node representing a state in a 
 * transfer graph, we should know next states to make our
 * automaton engine work.
 */
typedef struct AutomatonNode {
    TokenType cur;
    int next_num;
    char *err_msg;
    TokenType nexts[];
} AutomatonNode;

const AutomatonNode number_node = {
    .cur = TOKEN_NUMBER,
    .next_num = 4,
    .nexts = {TOKEN_BINOP, TOKEN_RPAREN, TOKEN_SEMICOLON, TOKEN_END},
    .err_msg = "invalid token after number."
};

const AutomatonNode dsymbol_node = {
    .cur = TOKEN_DSYMBOL,
    .next_num = 2,
    .nexts = {TOKEN_SEMICOLON, TOKEN_END},
    .err_msg = "invalid token after declaration."
};

const AutomatonNode rsymbol_node = {
    .cur = TOKEN_RSYMBOL,
    .next_num = 4,
    .nexts = {TOKEN_BINOP, TOKEN_RPAREN, TOKEN_SEMICOLON},
    .err_msg = "invalid token after number."
};

const AutomatonNode lsymbol_node = {
    .cur = TOKEN_LSYMBOL,
    .next_num = 2,
    .nexts = {TOKEN_LPAREN, TOKEN_ASSIGN},
    .err_msg = "expected a function call or assignment."
};

const AutomatonNode unop_node = {
    .cur = TOKEN_UNOP,
    .next_num = 3,
    .nexts = {TOKEN_LPAREN, TOKEN_RSYMBOL, TOKEN_NUMBER},
    .err_msg = "invalid token after unary operator."
};

const AutomatonNode assign_node = {
    .cur = TOKEN_ASSIGN,
    .next_num = 4,
    .nexts = {TOKEN_LPAREN, TOKEN_NUMBER, TOKEN_RSYMBOL, TOKEN_UNOP},
    .err_msg = "expected a function call or assignment."
};

const AutomatonNode binop_node = {
    .cur = TOKEN_BINOP,
    .next_num = 3,
    .nexts = {TOKEN_NUMBER, TOKEN_RSYMBOL, TOKEN_LPAREN},
    .err_msg = "invalid token after binary operator."
};

const AutomatonNode end_node = {
    .cur = TOKEN_END,
    .next_num = 0,
    .nexts = {},
    .err_msg = NULL
};

const AutomatonNode semicolon_node = {
    .cur = TOKEN_SEMICOLON,
    .next_num = 2,
    .nexts = {TOKEN_LSYMBOL, TOKEN_DECL},
    .err_msg = "expected a declaration or assignment or function call."
};

const AutomatonNode decl_node = {
    .cur = TOKEN_DECL,
    .next_num = 1,
    .nexts = {TOKEN_DSYMBOL},
    .err_msg = "expected a symbol after declaration keyword."
};

const AutomatonNode lparen_node = {
    .cur = TOKEN_LPAREN,
    .next_num = 5,
    .nexts = {TOKEN_NUMBER, TOKEN_RPAREN, TOKEN_LPAREN, TOKEN_RSYMBOL, TOKEN_UNOP},
    .err_msg = "expected a valid expression after '('."
};

const AutomatonNode rparen_node = {
    .cur = TOKEN_RPAREN,
    .next_num = 4,
    .nexts = {TOKEN_BINOP, TOKEN_SEMICOLON, TOKEN_RPAREN, TOKEN_END},
    .err_msg = "invalid token after ')'."
};

/**
 * be aware that we use a TokenType enum to index this array
 * so the order of nodes must be the same as what in the TokenType enum
 */
const AutomatonNode *automaton_nodes[12] = {
    &number_node,
    &dsymbol_node,
    &rsymbol_node,
    &lsymbol_node,
    &unop_node,
    &assign_node,
    &binop_node,
    &end_node,
    &semicolon_node,
    &decl_node,
    &lparen_node,
    &rparen_node,
};

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
bool parse_symbol(Project *proj, Token *t) {
    char symbol[MAX_TOKEN_SIZE + 1] = {0};

    // deal with the situation where a keyword may be recognized as a symbol
    if (peek_pattern(REGEX_DECL, symbol, proj->source_buffer) == TOKEN_MATCHED) {
        return false;
    }

    if (match_pattern(REGEX_SYM, symbol, proj->source_buffer) != TOKEN_MATCHED) {
        return false;
    }

    size_t token_len = strlen(symbol);
    t->content.name_idx = strtab_add(proj->strtab, symbol, token_len);
    return true;
}

/**
 * expect a semicolon in proj->source_buffer
 */ 
bool parse_semicolon(Project *proj, Token *t) {
    return token_unwrap(match_str(";", proj->source_buffer), proj->source_buffer->cur_line) == TOKEN_MATCHED;
}

/**
 * expect a eof signature
 */
bool parse_end(Project *proj, Token *t) {
    return token_unwrap(match_str(".", proj->source_buffer), proj->source_buffer->cur_line) == TOKEN_MATCHED;
}

bool parse_assign(Project *proj, Token *t) {
    return token_unwrap(match_str("=", proj->source_buffer), proj->source_buffer->cur_line) == TOKEN_MATCHED;
}

bool parse_lparen(Project *proj, Token *t) {
    return token_unwrap(match_str("(", proj->source_buffer), proj->source_buffer->cur_line) == TOKEN_MATCHED;
}

bool parse_rparen(Project *proj, Token *t) {
    return token_unwrap(match_str(")", proj->source_buffer), proj->source_buffer->cur_line) == TOKEN_MATCHED;
}

bool parse_unop(Project *proj, Token *t) {
    char opc;
    if (match_pattern(REGEX_UNOP, &opc, proj->source_buffer) != TOKEN_MATCHED) {
        return false;
    }

    switch (opc) {
        case '+':
            t->content.un_op = UNOP_POS;
            break;
        case '-':
           t->content.un_op = UNOP_NEG;
            break;
        default:
            panic(proj->source_buffer->cur_line, "unsupported unary operator.");
    }
    return true;
}

bool parse_binop(Project *proj, Token *t) {
    char opc;
    if (match_pattern(REGEX_BINOP, &opc, proj->source_buffer) != TOKEN_MATCHED) {
        return false;
    }

    switch (opc) {
        case '+':
            t->content.bin_op = BINOP_ADD;
            break;
        case '-':
            t->content.bin_op = BINOP_SUB;
            break;
        case '*':
            t->content.bin_op = BINOP_MUL;
            break;
        case '/':
            t->content.bin_op = BINOP_DIV;
            break;
        default:
            panic(proj->source_buffer->cur_line, "unsupported binary operator.");
    }
    return true;
}

bool parse_number(Project *proj, Token *t) {
    char buf[MAX_TOKEN_SIZE + 1] = {0};
    char *endptr = &buf[MAX_TOKEN_SIZE];
    if (match_pattern(REGEX_NUM, buf, proj->source_buffer) != TOKEN_MATCHED) {
        return false;
    }

    t->content.number = strtold(buf, &endptr);
    return true;
}

bool parse_decl(Project *proj, Token *t) {
    char buf[MAX_TOKEN_SIZE + 1] = {0};
    if (match_pattern(REGEX_DECL, buf, proj->source_buffer) != TOKEN_MATCHED) {
        return false;
    }
    if (buf[0] == 'i') {
        t->content.decl = DECL_INT;
    } else {
        t->content.decl = DECL_FLOAT;
    }
    return true;
}

/**
 * a dispatch table for parser functions
 * note each function index in this array is corresponded to its parsing type
 */
const bool (*parser_funcs[12])(Project *, Token *) = {
    parse_number,
    parse_symbol,
    parse_symbol,
    parse_symbol,
    parse_unop,
    parse_assign,
    parse_binop,
    parse_end,
    parse_semicolon,
    parse_decl,
    parse_lparen,
    parse_rparen
};

bool try_parse_all(const AutomatonNode *node, Project *proj, Token *t) {
    for (int i = 0; i < node->next_num; i++) {
        int cur_type = node->nexts[i];
        if (parser_funcs[cur_type](proj, t)) {
            t->type = cur_type;
            t->line = proj->source_buffer->cur_line;
            return true;
        }
    }
    return false;
}


void _tokenize(Project *proj, TokenType state) {
    if (state == TOKEN_END) {
        return;
    }

    Token *t = pool_use(proj->tokens);

    if (!try_parse_all(automaton_nodes[state], proj, t)) {
        panic(proj->source_buffer->cur_line, automaton_nodes[state]->err_msg);
    }

    // expect a tail recursion elimination here
    _tokenize(proj, t->type);
}

/**
 * check whether the parentheses in source code matches
 * if not match, exit with error message
 */
void paren_check(Project *proj) {
    Token *t;
    Token *lparen = NULL;
    FOR_EACH(t, proj->tokens) {
        switch (t->type) {
            case TOKEN_LPAREN:
                lparen = t;
                break;
            case TOKEN_RPAREN:
                lparen->content.rparen_idx = pool_idx(proj->tokens, t);
                lparen = NULL;
                break;
            case TOKEN_SEMICOLON:
                if (lparen != NULL) {
                    panic(t->line, "parentheses not match.");
                }
                break;
            case TOKEN_END:
                if (lparen != NULL) {
                    panic(t->line, "parentheses not match.");
                }
                reset_iter(proj->tokens);
                return;
        }
    }
}

// entrance of the automaton for parsing tokens
void tokenize(Project *proj) {
    Token *t = pool_use(proj->tokens);
    char first_decl[8];
    if (match_pattern(REGEX_DECL, first_decl, proj->source_buffer) != TOKEN_MATCHED) {
        log_error("the program must start with a declaration.");
        exit(-1);
    }

    /** 
     * initialize the initial state
     * note that due to the grammar limitation, the program
     * must start with a declaration
     */
    t->type = TOKEN_DECL;
    if (first_decl[0] == 'i') {
        t->content.decl = DECL_INT;
    } else {
        t->content.decl = DECL_FLOAT;
    }

    _tokenize(proj, t->type);
    paren_check(proj);
}

