/**
 * @file term_list.c
 *
 * @brief
 *
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.1
 * @date 2025-04-06
 * @copyright Copyright (c) 2025
 */

#include "term_list.h"
#include "alloc.h"

term_list_t* create_term_list(void) {

    return (term_list_t*)create_ptr_list();
}

void destroy_term_list(term_list_t* lst) {

    if(lst != NULL) {
        term_item_t* item;
        int mark = 0;
        while(NULL != (item = iterate_term_list(lst, &mark)))
            destroy_term_list(item);

        destroy_ptr_list((ptr_list_t*)lst);
    }
}

void append_term_list(term_list_t* lst, term_item_t* item) {

    append_ptr_list((ptr_list_t*)lst, (void*)item);
}

term_item_t* iterate_term_list(term_list_t* lst, int* mark) {

    return (term_item_t*)iterate_ptr_list((ptr_list_t*)lst, mark);
}

term_item_t* create_term_item(const char* term, const char* tok) {

    term_item_t* ptr = _ALLOC_TYPE(term_item_t);
    ptr->term = _COPY_STRING(term);
    ptr->token = _COPY_STRING(tok);

    return ptr;
}

void destroy_term_item(term_item_t* item) {

    if(item != NULL) {
        _FREE(item->term);
        _FREE(item->token);
        _FREE(item);
    }
}

