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
#include "render_ast_header.h"
#include "render_ast_source.h"
#include "render_parser_header.h"
#include "render_parser_source.h"
#include "render_init.h"

 void render(void) {

    render_init();
    render_ast_header();
    render_ast_source();
    render_parser_header();
    render_parser_source();
 }


