/**
 * @file render_ast_source.c
 *
 * @brief Render the AST source file
 *
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.1
 * @date 2025-04-10
 * @copyright Copyright (c) 2025
 */
#include "render_init.h"

void render_ast_source(void) {

    char buffer[1024];
    FILE* fp = fopen(make_ast_fname(buffer, sizeof(buffer), "ast.c"), "w");

    render_template(ast_c_template, fp, render_table);
}
