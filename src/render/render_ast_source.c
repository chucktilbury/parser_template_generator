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

    FILE* fp = fopen("ast.c", "w");

    render("../templates/ast.c.txt", fp, render_table);
}
