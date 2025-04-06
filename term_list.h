/**
 * @file term_list.h
 *
 * @brief Manage the terminal list
 *
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.1
 * @date 2025-04-06
 * @copyright Copyright (c) 2025
 */

#ifndef _TERM_LIST_H_
#define _TERM_LIST_H_

#include "ptrlist.h"

typedef ptr_list_t term_list_t;

typedef struct {
    const char* term;
    const char* token;
} term_item_t;

term_list_t* create_term_list(void);
void destroy_term_list(term_list_t* lst);
void append_term_list(term_list_t* lst, term_item_t* item);
term_item_t* iterate_term_list(term_list_t* lst, int* mark);

term_item_t* create_term_item(const char* term, const char* tok);
void destroy_term_item(term_item_t* item);

#endif /* _TERM_LIST_H_ */
