#include "string_list.h"

StringList string_list_new() {
    StringList list;
    VKT_LIST_HELPER_INIT_LIST(list, str_t, strings, 64);
    return list;
}

void string_list_push(StringList *list, str_t str) {
    VKT_LIST_HELPER_PUSH_ELEMENT(list, str_t, strings, str, 2);
}

void string_list_delete(StringList *list) {
    free(list->strings);
}
