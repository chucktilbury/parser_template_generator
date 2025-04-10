#ifndef _RENDER_H_
#define _RENDER_H_

#include <stdio.h>
#include "hash.h"

typedef struct {
    const char* name;
    void* data;
    void (*render_func)(FILE*, void*);
} render_item_t;

typedef hash_table_t render_table_t;

render_table_t* create_render(void);
void add_render(render_table_t* ptr, render_item_t* item);
void render(const char* template, FILE* outf, render_table_t* ptr);

#endif /* _RENDER_H_ */
