#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "render.h"
#include "template.h"
#include "alloc.h"
#include "errors.h"

template_t* create_render(FILE* inf, FILE* outf) {

    template_t* ptr = _ALLOC_TYPE(template_t);

    ptr->infile  = inf;
    ptr->outfile = outf;
    ptr->table   = create_hashtable();

    return ptr;
}

void render(template_t* ptr) {

    template_item_t* item = NULL;
    const char* name;

    template_in  = ptr->infile;
    template_out = ptr->outfile;

    while(template_lex()) {
        name = template_buffer;
        ASSERT(find_hashtable(ptr->table, name, (void**)&item),
               "error: render key \"%s\" not found.\n", name);
        item->render_func(ptr->outfile, item->data);
    }
}

void add_render(template_t* ptr, template_item_t* item) {

    insert_hashtable(ptr->table, item->name, (void*)item);
}
