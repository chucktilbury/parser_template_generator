/**
 * @file nnterm_list.c
 *
 * @brief
 *
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.1
 * @date 2025-04-06
 * @copyright Copyright (c) 2025
 */

#include "nterm_list.h"
#include "alloc.h"

nterm_list_t* create_nterm_list(void) {

    return (nterm_list_t*)create_ptr_list();
}

void destroy_nterm_list(nterm_list_t* lst) {

    if(lst != NULL) {
        nterm_item_t* item;
        int mark = 0;
        while(NULL != (item = iterate_nterm_list(lst, &mark)))
            destroy_nterm_item(item);

        destroy_ptr_list((ptr_list_t*)lst);
    }
}

void append_nterm_list(nterm_list_t* lst, nterm_item_t* item) {

    append_ptr_list((ptr_list_t*)lst, (void*)item);
}

nterm_item_t* iterate_nterm_list(nterm_list_t* lst, int* mark) {

    return (nterm_item_t*)iterate_ptr_list((ptr_list_t*)lst, mark);
}

nterm_item_t* create_nterm_item(const char* nterm) {

    nterm_item_t* ptr = _ALLOC_TYPE(nterm_item_t);
    ptr->nterm        = _COPY_STRING(nterm);

    return ptr;
}

void destroy_nterm_item(nterm_item_t* item) {

    if(item != NULL) {
        _FREE(item->nterm);
        _FREE(item);
    }
}
