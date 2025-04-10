#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "render.h"
#include "alloc.h"
#include "hash.h"
#include "errors.h"


render_table_t* create_render(void) {

    return (render_table_t*)create_hashtable();
}

void add_render(render_table_t* ptr, render_item_t* item) {

    insert_hashtable(ptr, item->name, item);
}

void render(const char* template, FILE* outf, render_table_t* ptr) {

    FILE* inf = fopen(template, "r");
    ASSERT(inf != NULL, "cannot open template file: \"%s\": %s\n", template, strerror(errno));

    int line_no = 1;

    char name[32];
    int name_idx = 0;

    int ch;
    int state = 0;

    while(EOF != (ch = fgetc(inf))) {
        switch(state) {
            case 0:
                if(ch == '\n') {
                    line_no++;
                    fputc(ch, outf);
                }
                else if(ch == '{')
                    state = 1;
                else
                    fputc(ch, outf);
                break;

            case 1:
                if(ch == '{') {
                    memset(name, 0, sizeof(name));
                    name_idx = 0;
                    state = 2;
                }
                else {
                    fputc('{', outf);
                    fputc(ch, outf);
                    state = 0;
                }
                break;

            case 2: // found a marker, read the name
                if(ch == '}')
                    state = 3;
                else if(isalnum(ch)) {
                    name[name_idx] = ch;
                    name_idx++;
                    if(name_idx >= sizeof(name))
                        FATAL("name is too long on line %d in template %s\n", line_no, template);
                }
                else
                    FATAL("invalid character '%c' in a marker name on line %d in template \"%s\"\n", ch, line_no, template);
                break;

            case 3:
                if(ch == '}')
                    state = 4;
                else
                    FATAL("a invalid '}' was found on line %d in the template \"%s\"\n", line_no, template);
                break;

            case 4: { // process a valid name
                    render_item_t* item = NULL;
                    if(find_hashtable(ptr, name, (void**)&item)) {
                        // call the render function
                        item->render_func(outf, item->data);
                    }
                    else {
                        // if the tag is not found then output it as a tag but no error
                        fprintf(outf, "{{%s}}", name);
                    }
                }
                fputc(ch, outf);
                state = 0;
                break;

            default:
                FATAL("unknown state in render engine: %d", state);
        }
    }

    fclose(inf);
}
