/**
 * @file render.c
 *
 * @brief
 *
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.1
 * @date 2025-04-12
 * @copyright Copyright (c) 2025
 */
#include "render_init.h"

static void rend_file(const char* buf, const char* name) {

    char buffer[1024];
    FILE* fp = fopen(make_fname(buffer, sizeof(buffer), name), "w");

    render_template(buf, fp, render_table);
    fprintf(fp, "\n\n");
    fclose(fp);
}

// static void render_ast_header(void) {

//     char buffer[1024];
//     FILE* fp = fopen(make_ast_fname(buffer, sizeof(buffer), "ast.h"), "w");

//     render_template(ast_h_template, fp, render_table);
//     fprintf(fp, "\n\n");
//     fclose(fp);
// }

// static void render_ast_source(void) {

//     char buffer[1024];
//     FILE* fp = fopen(make_ast_fname(buffer, sizeof(buffer), "ast.c"), "w");

//     render_template(ast_c_template, fp, render_table);
//     fprintf(fp, "\n\n");
//     fclose(fp);
// }

// static void render_parser_header(void) {

//     char buffer[1024];
//     FILE* fp = fopen(make_parser_fname(buffer, sizeof(buffer), "parser.h"), "w");

//     render_template(parser_h_template, fp, render_table);
//     fprintf(fp, "\n\n");
//     fclose(fp);
// }

// static void render_parser_source(void) {

//     char buffer[1024];
//     FILE* fp = fopen(make_parser_fname(buffer, sizeof(buffer), "parser.c"), "w");

//     render_template(parser_c_template, fp, render_table);
//     fprintf(fp, "\n\n");
//     fclose(fp);
// }

 void render(void) {

    render_init();
    rend_file(ast_h_template, "ast.h");
    rend_file(ast_c_template, "ast.c");
    rend_file(parser_h_template, "parser.h");
    rend_file(parser_c_template, "parser.c");
    // render_ast_header();
    // render_ast_source();
    // render_parser_header();
    // render_parser_source();
 }


