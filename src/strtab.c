#include "strtab.h"

void delete_strtab(StringTable *s) {
    free(s->buf);
    free(s->offsets);
    free(s);
}

StringTable *create_strtab() {
    StringTable *strtab = s_malloc(sizeof(StringTable));
    strtab->buf_size = 0x200;
    strtab->buf = s_malloc(strtab->buf_size);
    strtab->offset_max_num = 100;
    strtab->offsets = s_malloc(strtab->offset_max_num * sizeof(size_t));
    strtab->offsets[0] = 0;
    strtab->offset_num = 0;
    strtab->free = delete_strtab;
    return strtab;
}

char *strtab_get(StringTable *s, size_t idx) {
    // simple bound check
    if (idx >= s->offset_num) {
        log_error("internal error: strtab access out of bound (%d/%d).", idx, s->offset_num);
        exit(-1);
    }

    return &s->buf[s->offsets[idx]];
}

void strtab_add(StringTable *s, char *str, unsigned int len) {
    size_t offset = s->offsets[s->offset_num];
    
    // deal with the condition when buffer is not large enough
    size_t next_offset = offset + len + 1;
    if (next_offset > s->buf_size) {
        s->buf_size *= 2;
        s->buf = (char *) s_realloc(s->buf, s->buf_size);
    }

    strncpy(&s->buf[offset], str, len);
    s->buf[offset + len] = '\00';
    s->offset_num++;
    
    // deal with the condition when offsets array is not large enough
    if (s->offset_num > s->offset_max_num) {
        s->offset_max_num *= 2;
        s->offsets = (size_t *) s_realloc(s->offsets, s->offset_max_num);
    }

    s->offsets[s->offset_num] = offset + len + 1;
}
