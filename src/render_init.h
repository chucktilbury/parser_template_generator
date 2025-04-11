/**
 * @file render_init.h
 *
 * @brief Init the render data structures.
 *
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.1
 * @date 2025-04-10
 * @copyright Copyright (c) 2025
 */
#ifndef _RENDER_INIT_H_
#define _RENDER_INIT_H_

#include "render.h"

#include "term_list.h"
#include "nterm_list.h"

extern render_table_t* render_table;

void render_init(term_list_t*, nterm_list_t*);

#endif /* _RENDER_INIT_H_ */
