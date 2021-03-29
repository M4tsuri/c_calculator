#ifndef PROJECT_H
#define PROJECT_H
#include "buffer.h"
#include "strtab.h"
#include "ast_types.h"
#include "symtab.h"
#include <stdbool.h>

typedef struct Project {
    Pool *tokens;
    Program *program;
    StringTable *strtab;
    SymbolTable *symtab;
    FILE *source_file;
    FILE *output_file;
    // source code buffer
    Buffer *source_buffer;
    void (*free)(struct Project *);
} Project;

Project *create_project(int argc, char **argv);

#endif