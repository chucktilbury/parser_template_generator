/**
 * @file nterm_list.h
 *
 * @brief Manage non-terminals data structure list.
 *
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.1
 * @date 2025-04-06
 * @copyright Copyright (c) 2025
 */

#ifndef _NTERM_LIST_H_
#define _NTERM_LIST_H_

#include "ptrlist.h"
#include "strgs.h"

typedef ptr_list_t nterm_list_t;

typedef struct {
    string_t* nterm;
    string_t* type;
} nterm_item_t;

nterm_list_t* create_nterm_list(void);
void destroy_nterm_list(nterm_list_t* lst);
void append_nterm_list(nterm_list_t* lst, nterm_item_t* item);
nterm_item_t* iterate_nterm_list(nterm_list_t* lst, int* mark);
nterm_item_t* index_nterm_list(nterm_list_t* lst, int idx);

nterm_item_t* create_nterm_item(string_t* nterm, string_t* type);
void destroy_nterm_item(nterm_item_t* item);


#endif /* _NTERM_LIST_H_ */
