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
#ifndef _STRG_LIST_H_
#define _STRG_LIST_H_

#include "strgs.h"
#include "ptr_list.h"

typedef ptr_list_t string_list_t;

string_list_t* create_string_list(void);
void destroy_string_list(string_list_t* lst);
void append_string_list(string_list_t* lst, string_t* str);
string_t* index_string_list(string_list_t* lst, int index);
string_t* iterate_string_list(string_list_t* lst, int* post);
int len_string_list(string_list_t* lst);
void sort_string_list(string_list_t* lst);
int find_string_list(string_list_t* lst, string_t* key);

void add_string_list(string_list_t* lst, string_t* str);

#endif /* _STRG_LIST_H_ */
