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

#include "render_init.h"
#include "lists.h"

#define DEV_NAME "Chuck Tilbury"

render_table_t* render_table = NULL;

static void _ctime(FILE* fp, void* ptr) {

    (void)ptr;
    time_t t             = time(NULL);
    char* tpt            = ctime(&t);
    tpt[strlen(tpt) - 1] = '\0';
    fputs(tpt, fp);
}

static void _dev_name(FILE* fp, void* ptr) {

    (void)ptr;
    fprintf(fp, DEV_NAME);
}

static void _first_node(FILE* fp, void* ptr) {

    master_list_t* lst = (master_list_t*)ptr;

    nterm_item_t* item = lst->first_nterm;
    fprintf(fp, "%s", item->nterm->buffer);
}

static void _type_to_str(FILE* fp, void* ptr) {

    master_list_t* lst = (master_list_t*)ptr;

    int mark = 0;
    nterm_item_t* item;

    while(NULL != (item = iterate_nterm_list(lst->nterm_list, &mark))) {
        lst->nterm_idx = mark - 1;
        render_template(ast_type_to_str_template, fp, render_table);
        //fprintf(fp, "\n");
    }
}

static void _type_to_size(FILE* fp, void* ptr) {

    master_list_t* lst = (master_list_t*)ptr;

    int mark = 0;
    nterm_item_t* item;

    while(NULL != (item = iterate_nterm_list(lst->nterm_list, &mark))) {
        lst->nterm_idx = mark - 1;
        render_template(ast_type_to_size_template, fp, render_table);
        //fprintf(fp, "\n");
    }
}

static void _type_list(FILE* fp, void* ptr) {

    master_list_t* lst = (master_list_t*)ptr;

    int mark = 0;
    nterm_item_t* item;
    int val = 256;

    while(NULL != (item = iterate_nterm_list(lst->nterm_list, &mark))) {
        fprintf(fp, "    %s = %d", item->type->buffer, val);
        if((size_t)mark + 1 <= lst->nterm_list->len)
            fprintf(fp, ",\n");
        val++;
    }
}

static void _data_structures(FILE* fp, void* ptr) {

    master_list_t* lst = (master_list_t*)ptr;

    int mark = 0;
    nterm_item_t* item;

    while(NULL != (item = iterate_nterm_list(lst->nterm_list, &mark))) {
        lst->nterm_idx = mark - 1;
        render_template(ast_data_struct_template, fp, render_table);
        fprintf(fp, "\n");
    }
}

static void _func_defs(FILE* fp, void* ptr) {

    master_list_t* lst = (master_list_t*)ptr;

    int mark = 0;
    nterm_item_t* item;

    while(NULL != (item = iterate_nterm_list(lst->nterm_list, &mark))) {
        lst->nterm_idx = mark - 1;
        render_template(ast_func_def_template, fp, render_table);
        fprintf(fp, "\n");
    }
}

static void _function_protos(FILE* fp, void* ptr) {


    master_list_t* lst = (master_list_t*)ptr;

    int mark = 0;
    nterm_item_t* item;

    while(NULL != (item = iterate_nterm_list(lst->nterm_list, &mark))) {
        lst->nterm_idx = mark - 1;
        render_template(ast_func_proto_template, fp, render_table);
        //fprintf(fp, "\n");
    }
}

static void _nterm_name(FILE* fp, void* ptr) {

    master_list_t* lst = (master_list_t*)ptr;
    nterm_item_t* item = index_nterm_list(lst->nterm_list, lst->nterm_idx);
    fprintf(fp, "%s", item->nterm->buffer);
}

static void _type_name(FILE* fp, void* ptr) {

    master_list_t* lst = (master_list_t*)ptr;
    nterm_item_t* item = index_nterm_list(lst->nterm_list, lst->nterm_idx);
    fprintf(fp, "%s", item->type->buffer);
}

/*
 * This is a list of all of the objects that could be rendered by template name
 *
 * -- generics
 * ctime
 * dev_name
 *
 * -- ast.c.txt
 * ast_first_node
 * ast_type_to_str
 * ast_type_to_size
 *
 * -- ast.h.txt
 * ast_type_list
 * ast_data_structures
 * ast_function_protos
 *
 * -- ast_data_struct.txt
 * ast_nterm_name
 * ast_struct_elements
 *
 * -- ast_func_proto.txt
 * ast_nterm_name
 *
 * -- ast_type_to_size.txt
 * ast_type_name
 * ast_nterm_name
 *
 * -- ast_type_to_str.txt
 * ast_type_name
 * ast_nterm_name
 *
 */
static void make_render_table(void) {

    render_table = create_render_table();

    add_render(render_table, create_render_item("ctime", NULL, _ctime));
    add_render(render_table, create_render_item("dev_name", NULL, _dev_name));

    add_render(render_table, create_render_item("ast_type_list", master_list, _type_list));
    add_render(render_table, create_render_item("ast_data_structures", master_list, _data_structures));
    add_render(render_table, create_render_item("ast_function_protos", master_list, _function_protos));
    add_render(render_table, create_render_item("ast_type_name", master_list, _type_name));
    add_render(render_table, create_render_item("ast_nterm_name", master_list, _nterm_name));
    add_render(render_table, create_render_item("ast_type_to_size", master_list, _type_to_size));
    add_render(render_table, create_render_item("ast_type_to_str", master_list, _type_to_str));
    add_render(render_table, create_render_item("ast_first_node", master_list, _first_node));
    add_render(render_table, create_render_item("ast_function_defs", master_list, _func_defs));
}


// NOTE that these values will be supplied on the command line and have
// reasonable defaults but for now, they are hard-coded.
void make_render_dirs(void) {

    remove("./out");
    mkdir("./out", 0777);
    mkdir("./out/ast", 0777);
    mkdir("./out/parser", 0777);
    mkdir("./out/scanner", 0777);
}

const char* make_ast_fname(char* buf, size_t size, const char* base) {

    snprintf(buf, size, "./out/ast/%s", base);
    return _COPY_STRING(buf);
}

const char* make_parser_fname(char* buf, size_t size, const char* base) {

    snprintf(buf, size, "./out/parser/%s", base);
    return _COPY_STRING(buf);
}

const char* make_scanner_fname(char* buf, size_t size, const char* base) {

    snprintf(buf, size, "./out/scanner/%s", base);
    return _COPY_STRING(buf);
}

void render_init(void) {

    make_render_dirs();
    make_render_table();
}


