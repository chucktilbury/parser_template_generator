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

 #include "render_init.h"

render_table_t* render_table = NULL;

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

void render_init(term_list_t* terms, nterm_list_t* nterms) {

 }