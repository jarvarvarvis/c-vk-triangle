#ifndef UTIL_STRING_LIST_H
#define UTIL_STRING_LIST_H

#include "list_helper.h"

typedef char *str_t;

typedef struct {
    VKT_LIST_HELPER_STRUCT_MEMBERS(str_t, strings);
} StringList;

StringList string_list_new();
void string_list_push(StringList *list, str_t str);
void string_list_delete(StringList *list);

#endif /* UTIL_STRING_LIST_H */
