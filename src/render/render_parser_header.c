/**
 * @file render_parser_header.c
 *
 * @brief
 *
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.1
 * @date 2025-04-12
 * @copyright Copyright (c) 2025
 */
#include "render_init.h"

void render_parser_header(void) {

    char buffer[1024];
    FILE* fp = fopen(make_parser_fname(buffer, sizeof(buffer), "parser.h"), "w");

    render_template(parser_h_template, fp, render_table);
}
