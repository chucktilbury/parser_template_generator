
#include "alloc.h"
#include "ptr_list.h"

ptr_list_t* create_ptr_list(void) {

    ptr_list_t* ptr = _ALLOC_TYPE(ptr_list_t);
    ptr->len        = 0;
    ptr->cap        = 1 << 3;
    ptr->buffer     = _ALLOC_ARRAY(void*, ptr->cap);

    return ptr;
}

void destroy_ptr_list(ptr_list_t* lst) {

    if(lst != NULL) {
        _FREE(lst->buffer);
        _FREE(lst);
    }
}

void append_ptr_list(ptr_list_t* lst, void* ptr) {

    if(lst->len + 1 > lst->cap) {
        lst->cap <<= 1;
        lst->buffer = _REALLOC_ARRAY(lst->buffer, void*, lst->cap);
    }

    lst->buffer[lst->len] = ptr;
    lst->len++;
}

#include <stdio.h>

void* index_ptr_list(ptr_list_t* lst, int index) {

    if(index >= 0 && (size_t)index < lst->len)
        return lst->buffer[index];
    else
        return NULL;
}

void push_ptr_list(ptr_list_t* lst, void* ptr) {

    append_ptr_list(lst, ptr);
}

void* pop_ptr_list(ptr_list_t* lst) {

    if(lst->len > 0) {
        lst->len--;
        return lst->buffer[lst->len];
    }
    else
        return NULL;
}

void* peek_ptr_list(ptr_list_t* lst) {

    if(lst->len > 0) {
        return lst->buffer[lst->len - 1];
    }
    else
        return NULL;
}

void* iterate_ptr_list(ptr_list_t* lst, int* post) {

    void* ptr = NULL;

    if((*post >= 0) && ((size_t)*post < lst->len)) {
        ptr   = lst->buffer[*post];
        *post = *post + 1;
    }

    return ptr;
}

int len_ptr_list(ptr_list_t* lst) {

    return (int)lst->len;
}
