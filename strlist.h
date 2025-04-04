/**
 * @file strlist.h
 *
 * @brief Thin wrapper for pointer lists.
 *
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.1
 * @date 2025-04-01
 * @copyright Copyright (c) 2025
 */
#ifndef _STRLIST_H_
#define _STRLIST_H_

#include "strbuf.h"
#include "ptrlist.h"

typedef ptr_list_t str_list_t;

str_list_t* create_str_list(void);
void append_str_list(str_list_t* lst, str_buf_t* str);
str_buf_t* index_str_list(str_list_t* lst, int index);
str_buf_t* iterate_str_list(str_list_t* lst, int* post);
int len_str_list(str_list_t* lst);

void add_str_list(str_list_t* lst, str_buf_t* str);

#endif /* _STRLIST_H_ */
