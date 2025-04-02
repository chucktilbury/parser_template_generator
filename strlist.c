/**
 * @file strlist.c
 *
 * @brief Thin wrapper for pointer lists.
 *
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.1
 * @date 2025-04-01
 * @copyright Copyright (c) 2025
 */
#include "strlist.h"

str_list_t* create_str_list(void) {

    return (str_list_t*)create_ptr_list();
}

void append_str_list(str_list_t* lst, str_buf_t* str) {

    append_ptr_list((ptr_list_t*)lst, (void*)str);
}

str_buf_t* index_str_list(str_list_t* lst, int index) {

    return (str_buf_t*)index_ptr_list((ptr_list_t*)lst, index);
}

str_buf_t* iterate_str_list(str_list_t* lst, int* post) {

    return (str_buf_t*)iterate_ptr_list((ptr_list_t*)lst, post);
}

int len_str_list(str_list_t* lst) {

    return len_ptr_list((ptr_list_t*)lst);
}

