#ifndef VKT_UTIL_LIST_HELPER_H
#define VKT_UTIL_LIST_HELPER_H

#include <stddef.h>
#include <stdlib.h>

#include "concat.h"

// This macro produces an assignment of an expression to a constant with a specific type.
// It's used in the VKT_LIST_HELPER_INIT_LIST and VKT_LIST_HELPER_PUSH_ELEMENT macros to
// verify that they were properly called by the user and produce compiler errors in case
// of a mistake.
#define VKT_LIST_HELPER_CONST_TY_CHECK(ty, name, expr) \
    const ty VKT_CONCAT(_type_check_, name) = expr

#define VKT_LIST_HELPER_STRUCT_MEMBERS(ty, buffer_name) \
    ty *buffer_name; \
    size_t buffer_name##_len, buffer_name##_capacity

#define VKT_LIST_HELPER_INIT_LIST(list, ty, buffer_name, initial_cap) \
    VKT_LIST_HELPER_CONST_TY_CHECK(ty*, buffer_name, list.buffer_name); \
    list.buffer_name##_len = 0; \
    list.buffer_name##_capacity = initial_cap; \
    list.buffer_name = malloc(sizeof(ty) * list.buffer_name##_capacity);

#define VKT_LIST_HELPER_PUSH_ELEMENT(list_ptr, ty, buffer_name, pushed_elem_name, growth_factor) \
    VKT_LIST_HELPER_CONST_TY_CHECK(ty*, buffer_name, list_ptr->buffer_name); \
    VKT_LIST_HELPER_CONST_TY_CHECK(ty, elem_##buffer_name, pushed_elem_name); \
    if (list_ptr->buffer_name##_len >= list_ptr->buffer_name##_capacity) { \
        list_ptr->buffer_name##_capacity *= growth_factor; \
        list_ptr->buffer_name = realloc(list_ptr->buffer_name, sizeof(ty) * list_ptr->buffer_name##_capacity); \
    } \
    \
    list_ptr->buffer_name[list_ptr->buffer_name##_len] = pushed_elem_name; \
    list_ptr->buffer_name##_len++;

#endif /* VKT_UTIL_LIST_HELPER_H */
