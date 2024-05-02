#include "string_list.h"

#include <stdlib.h>

StringList string_list_new() {
    StringList list;
    list.len = 0;
    list.capacity = 64;
    list.strings = malloc(sizeof(str_t) * list.capacity);
    return list;
}

void string_list_push(StringList *list, str_t str) {
    if (list->len >= list->capacity) {
        list->capacity *= 2;
        list->strings = realloc(list->strings, sizeof(str_t) * list->capacity);
    }

    list->strings[list->len] = str;
    list->len++;
}

void string_list_delete(StringList *list) {
    free(list->strings);
}
