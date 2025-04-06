/**
 * @file lists.h
 *
 * @brief Traverse the AST to create the raw lists
 *
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.1
 * @date 2025-04-06
 * @copyright Copyright (c) 2025
 */
#ifndef _LISTS_H_
#define _LISTS_H_

#include <stdio.h>
#include "ast.h"
#include "strlist.h"

extern str_list_t* nterm_list;
extern str_list_t* term_list;
extern str_list_t* token_list;

void make_raw_lists(grammar_t* node);

#endif /* _LISTS_H_ */
