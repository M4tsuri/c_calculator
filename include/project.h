#include "buffer.h"
#include "pool.h"
#include "strtab.h"
#include "ast_types.h"
#include "token_types.h"

typedef struct Project {
    Pool *tokens;
    Program *program;
    StringTable *strtab;
    FILE *source_file;
    FILE *output_file;
    // source code buffer
    Buffer *source_buffer;
    void (*free)(struct Project *);
} Project;

Project *create_project(int argc, char **argv);