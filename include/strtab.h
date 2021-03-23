#include <stdlib.h>
#include "utils.h"

#define DEFAULT_STRTAB_MAX_ITEM 0x50
#define DEFAULT_STRTAB_MAX_SIZE 0x200

// put all strings in a program to a string table struct
typedef struct StringTable {
    char *buf;
    // the array containing each string's offset in buffer
    size_t *offsets;
    // the maximum number of strings in this table
    size_t offset_max_num;
    // the maximum size of all strings in this table
    size_t buf_size;
    // the number of strings in this table
    size_t offset_num;
    void (*free)(struct StringTable *);
} StringTable;

/**
 * create a StringTable instance
 */
StringTable *create_strtab();

/**
 * we put all strings in one buffer, so we need a method to locate a desired string.
 * so we manage the strtab with an array containing all strings' offset in buffer, so get a specified string, we just need to provide its index in offset array.
 * @param s: the string table
 * @param idx: the offset array index
 * @return a reference to out desired string
 */
char *strtab_get(StringTable *s, size_t idx);

/**
 * size of the offset array and buf is dynamic in our string table, when one of them may overflow during an add
 * we enlarge it with realloc
 * @param s: the string table
 * @param str: the orginal string
 * @param len: the length of the string
 */
void strtab_add(StringTable *s, char *str, unsigned int len);
