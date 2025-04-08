#ifndef _RENDER_H_
#define _RENDER_H_

#include <stdio.h>
#include "hash.h"

// defined in scanner.l
extern char template_buffer[];

typedef struct {
    const char* name;
    void* data;
    void (*render_func)(FILE*, void*);
} template_item_t;

typedef struct {
    FILE* infile;
    FILE* outfile;
    hash_table_t* table;
} template_t;

template_t* create_render(FILE* inf, FILE* outf);
void render(template_t* ptr);
void add_render(template_t* ptr, template_item_t* item);

#endif /* _RENDER_H_ */
