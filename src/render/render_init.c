/**
 * @file render_init.c
 *
 * @brief Init the render data structures
 *
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.1
 * @date 2025-04-10
 * @copyright Copyright (c) 2025
 */
#include <sys/stat.h>
#include <time.h>
#include <string.h>

#include "term_list.h"
#include "nterm_list.h"
#include "nterm_ds.h"
#include "templates.h"

#include "render_init.h"
#include "render_parser.h"
#include "lists.h"

#define DEV_NAME "Chuck Tilbury"

render_table_t* render_table = NULL;

static void _ctime(FILE* fp) {

    time_t t             = time(NULL);
    char* tpt            = ctime(&t);
    tpt[strlen(tpt) - 1] = '\0';
    fputs(tpt, fp);
}

static void _dev_name(FILE* fp) {

    fprintf(fp, DEV_NAME);
}

static void _file_name(FILE* fp) {

    fprintf(fp, "%s", master_list->current_file->buffer);
}

static void _file_header(FILE* fp) {

    render_template(file_header_template, fp, render_table);
}

static void _first_node(FILE* fp) {

    nterm_item_t* item = master_list->first_nterm;
    fprintf(fp, "%s", item->nterm->buffer);
}

static void _type_to_str(FILE* fp) {

    int mark = 0;
    nterm_item_t* item;

    while(NULL != (item = iterate_nterm_list(master_list->nterm_list, &mark))) {
        master_list->nterm_idx = mark - 1;
        render_template(ast_type_to_str_template, fp, render_table);
        //fprintf(fp, "\n");
    }
}

static void _type_to_size(FILE* fp) {

    int mark = 0;
    nterm_item_t* item;

    while(NULL != (item = iterate_nterm_list(master_list->nterm_list, &mark))) {
        master_list->nterm_idx = mark - 1;
        render_template(ast_type_to_size_template, fp, render_table);
        //fprintf(fp, "\n");
    }
}

static void _type_list(FILE* fp) {

    int mark = 0;
    nterm_item_t* item;
    int val = 512;

    while(NULL != (item = iterate_nterm_list(master_list->nterm_list, &mark))) {
        fprintf(fp, "    %s = %d", item->type->buffer, val);
        if((size_t)mark + 1 <= master_list->nterm_list->len)
            fprintf(fp, ",\n");
        val++;
    }
}

static void _data_structures(FILE* fp) {

    int mark = 0;
    nterm_item_t* item;

    while(NULL != (item = iterate_nterm_list(master_list->nterm_list, &mark))) {
        master_list->nterm_idx = mark - 1;
        render_template(ast_data_struct_template, fp, render_table);
        fprintf(fp, "\n");
    }
}

static void _ast_func_defs(FILE* fp) {

    int mark = 0;
    nterm_item_t* item;

    while(NULL != (item = iterate_nterm_list(master_list->nterm_list, &mark))) {
        master_list->nterm_idx = mark - 1;
        render_template(ast_func_def_template, fp, render_table);
        fprintf(fp, "\n");
    }
}

static void _ast_func_protos(FILE* fp) {

    int mark = 0;
    nterm_item_t* item;

    while(NULL != (item = iterate_nterm_list(master_list->nterm_list, &mark))) {
        master_list->nterm_idx = mark - 1;
        render_template(ast_func_proto_template, fp, render_table);
        //fprintf(fp, "\n");
    }
}

static void _parser_func_protos(FILE* fp) {

    int mark = 0;
    nterm_item_t* item;

    while(NULL != (item = iterate_nterm_list(master_list->nterm_list, &mark))) {
        master_list->nterm_idx = mark - 1;
        render_template(parser_func_proto_template, fp, render_table);
        //fprintf(fp, "\n");
    }
}

static void _nterm_name(FILE* fp) {

    nterm_item_t* item = index_nterm_list(master_list->nterm_list, master_list->nterm_idx);
    fprintf(fp, "%s", item->nterm->buffer);
}

static void _type_name(FILE* fp) {

    nterm_item_t* item = index_nterm_list(master_list->nterm_list, master_list->nterm_idx);
    fprintf(fp, "%s", item->type->buffer);
}

static void _token_type_list(FILE* fp) {

    int mark = 0;
    term_item_t* item;
    int val = 258;

    while(NULL != (item = iterate_term_list(master_list->term_list, &mark))) {
        fprintf(fp, "    %s = %d", item->token->buffer, val);
        if((size_t)mark + 1 <= master_list->term_list->len)
            fprintf(fp, ",\n");
        val++;
    }
}

static void _tok_type_to_str(FILE* fp) {

    int mark = 0;
    term_item_t* item;

    while(NULL != (item = iterate_term_list(master_list->term_list, &mark))) {
        fprintf(fp, "        (tok->type == %s)? \"%s\":\n", item->token->buffer, item->term->buffer);
    }
}

static void _ast_struct_elements(FILE* fp) {

    nterm_item_t* nterm = index_nterm_list(master_list->nterm_list, master_list->nterm_idx);
    nterm_ds_type_t* item;
    int mark = 0;

    while(NULL != (item = iterate_ptr_list(nterm->ds_names, &mark))) {
        fprintf(fp, "// name = %s: %d\n", item->name->buffer, item->type);
    }
}

static void _ast_func_implementation(FILE* fp) {

    fprintf(fp, "// ast implementation is TBD\n");
}

static void make_render_table(void) {

    render_table = create_render_table();

    add_render(render_table, create_render_item("ctime", _ctime));
    add_render(render_table, create_render_item("dev_name", _dev_name));
    add_render(render_table, create_render_item("file_name", _file_name));
    add_render(render_table, create_render_item("file_header", _file_header));

    add_render(render_table, create_render_item("ast_type_list", _type_list));
    add_render(render_table, create_render_item("ast_data_structures", _data_structures));
    add_render(render_table, create_render_item("ast_function_protos", _ast_func_protos));
    add_render(render_table, create_render_item("ast_type_name", _type_name));
    add_render(render_table, create_render_item("ast_nterm_name", _nterm_name));
    add_render(render_table, create_render_item("ast_type_to_size", _type_to_size));
    add_render(render_table, create_render_item("ast_type_to_str", _type_to_str));
    add_render(render_table, create_render_item("ast_first_node", _first_node));
    add_render(render_table, create_render_item("ast_function_defs", _ast_func_defs));
    add_render(render_table, create_render_item("ast_struct_elements", _ast_struct_elements));
    add_render(render_table, create_render_item("ast_func_implementation", _ast_func_implementation));

    add_render(render_table, create_render_item("parser_protos", _parser_func_protos));
    // functions defined in render_parser.c
    add_render(render_table, create_render_item("parser_funcs", render_parser_funcs));
    add_render(render_table, create_render_item("parse_func_implementation", render_parser_implementation));

    add_render(render_table, create_render_item("token_type_list", _token_type_list));
    add_render(render_table, create_render_item("tok_type_to_str", _tok_type_to_str));
}


// NOTE that these values will be supplied on the command line and have
// reasonable defaults but for now, they are hard-coded.
void make_render_dirs(void) {

    remove("./out");
    mkdir("./out", 0777);
    // mkdir("./out/ast", 0777);
    // mkdir("./out/parser", 0777);
    // mkdir("./out/scanner", 0777);
}

const char* make_fname(char* buf, size_t size, const char* base) {

    snprintf(buf, size, "./out/%s", base);
    if(master_list->current_file != NULL)
        destroy_string(master_list->current_file);
    master_list->current_file = create_string(buf);
    return master_list->current_file->buffer;
}

// const char* make_ast_fname(char* buf, size_t size, const char* base) {

//     snprintf(buf, size, "./out/ast/%s", base);
//     if(master_list->current_file != NULL)
//         destroy_string(master_list->current_file);
//     master_list->current_file = create_string(buf);
//     return master_list->current_file->buffer;
// }

// const char* make_parser_fname(char* buf, size_t size, const char* base) {

//     snprintf(buf, size, "./out/parser/%s", base);
//     if(master_list->current_file != NULL)
//         destroy_string(master_list->current_file);
//     master_list->current_file = create_string(buf);
//     return master_list->current_file->buffer;
// }

// const char* make_scanner_fname(char* buf, size_t size, const char* base) {

//     snprintf(buf, size, "./out/scanner/%s", base);
//     if(master_list->current_file != NULL)
//         destroy_string(master_list->current_file);
//     master_list->current_file = create_string(buf);
//     return master_list->current_file->buffer;
// }

void render_init(void) {

    make_render_dirs();
    make_render_table();
}


