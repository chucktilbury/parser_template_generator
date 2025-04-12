/**
 * @file render_ast_header.c
 *
 * @brief Render the AST header file
 *
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.1
 * @date 2025-04-10
 * @copyright Copyright (c) 2025
 */

#include "render_init.h"

void render_ast_header(void) {

    FILE* fp = fopen("ast.h", "w");

    render("../templates/ast.h.txt", fp, render_table);
}
