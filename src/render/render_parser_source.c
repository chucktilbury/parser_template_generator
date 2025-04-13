/**
 * @file render_parser_source.c
 *
 * @brief
 *
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.1
 * @date 2025-04-12
 * @copyright Copyright (c) 2025
 */
#include "render_init.h"

void render_parser_source(void) {

    char buffer[1024];
    FILE* fp = fopen(make_parser_fname(buffer, sizeof(buffer), "parser.c"), "w");

    render_template(parser_c_template, fp, render_table);
}
