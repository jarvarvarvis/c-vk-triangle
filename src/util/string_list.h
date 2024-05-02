#ifndef UTIL_STRING_LIST_H
#define UTIL_STRING_LIST_H

#include <stddef.h>

typedef char *str_t;

typedef struct {
    str_t *strings;
    size_t len, capacity;
} StringList;

StringList string_list_new();
void string_list_push(StringList *list, str_t str);
void string_list_delete(StringList *list);

#endif /* UTIL_STRING_LIST_H */
