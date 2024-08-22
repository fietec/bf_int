#ifndef _DYNARRAY_H
#define _DYNARRAY_H

/*
    dynarray.h by cdm ©2024

    Description:
        implementation of a custom dynamic array including basic functionality, such as:
            - create
            - append
            - print (printing of values using custom printing function)
            - free (freeing of values using custom freeing function)

    Resources:
        - stdio.h
        - stdlib.h
        - stdbool.h
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define DEFINE_DYNAMIC_ARRAY(type, name) \
typedef struct { \
    type* items; \
    size_t size; \
    size_t capacity; \
    void* (*f_func) (type);\
} name##_t; \
\
name##_t* name##_create(size_t capacity, void* (*f_func) (type)) { \
    name##_t* value = (name##_t*)calloc(1, sizeof(name##_t)); \
    if (value) { \
        value->items = (type*)calloc(capacity, sizeof(type)); \
        if (!value->items) { \
            free(value); \
            return NULL; \
        } \
        value->capacity = capacity; \
        value->size = 0; \
        value->f_func = f_func;\
    } \
    return value; \
} \
char name##_append(name##_t* da, type value){\
    if (da){\
        if (da->size >= da->capacity){\
            size_t new_capacity = da->capacity*2;\
            type* new_items = realloc(da->items, new_capacity*sizeof(type));\
            if (!new_items) return 1;\
            da->items = new_items;\
            da->capacity = new_capacity;\
        }   \
        da->items[da->size++] = value;\
        return 0;\
    }\
    return 1;\
}\
void name##_free(name##_t* da){\
    if (!da) return;\
    if (da->items && da->f_func){\
        for (size_t i=0; i<da->size; i++){\
            da->f_func(da->items[i]);\
        }\
        free(da->items);\
    }\
    free(da);\
}\
void name##_print(name##_t* da, bool full, void* (*p_func) (type)){\
    if (!da || !da->items) return;\
    printf("\nDynarray %s (Type: %s):\n[\n", #name "_t", #type);\
    for (size_t i=0; i<da->capacity; i++){\
        if (i < da->size) {\
            printf("  Index % 3u: ", i);\
            if (p_func) {p_func(da->items[i]);}\
            else {printf("Value of size %d bytes", sizeof(type));}\
            putchar('\n');\
        }\
        else if (full) {printf("  Index % 3u: None\n", i);}\
    }\
    puts("]");\
    printf("Total: %d values\n\n", da->size);\
}
#endif // _DYNARRAY_H