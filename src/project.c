#include "project.h"

void delete_project(Project *proj) {
    FREE(proj->source_buffer);
    FREE(proj->strtab);
    FREE(proj->tokens);
    FREE(proj->symtab);
    free(proj);
}

Project *create_project(int argc, char **argv) {
    Project *proj = (Project *) s_malloc(sizeof(Project));
    proj->source_file = get_src(argc, argv);
    proj->output_file = get_dest(argc, argv);

    // split source code into tokens
    proj->source_buffer = create_buffer(proj->source_file);
    proj->tokens = create_pool(sizeof(Token));
    proj->strtab = create_strtab();
    proj->symtab = create_symtab();
    proj->free = delete_project;
    return proj;
}
