#include "res_handle.h"
#include "utils.h"
#include <stdlib.h>

TOKEN_RES token_unwrap(TOKEN_RES res, unsigned line) {
    switch (res) {
        case TOKEN_MATCHED:
        case TOKEN_UNMATCH:
            return res;
        case TOKEN_UNEXPECTED_END:
            log_error("line %d: unexpected end of token.", line);
            exit(-1);
    }
}
