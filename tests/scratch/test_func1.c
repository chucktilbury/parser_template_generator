/**
 * @file ./out/parser/expr_equal.c
 *
 * This is a generated file. If you edit this file, don't run the generator
 * in this directory. Run it in a different one and then merge the results
 * using a tool like diff.
 *
 * @date Tue May 20 11:47:43 2025
 * @author Chuck Tilbury
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "parser_protos.h"



static inline int func_11(ast_expr_equal_t* retv, parser_state_t* pstate) {
    token_t* tok = get_token();
    if(tok->type == TOK_BANG_EQUAL || tok->type == TOK_NEQU) {
        retv->tok = tok;
        return STATE_MATCH;
    }
    else
        return STATE_NO_MATCH;
}

static inline int func_10(ast_expr_equal_t* retv, parser_state_t* pstate) {
    return func_11(retv, pstate);
}

static inline int func_9(ast_expr_equal_t* retv, parser_state_t* pstate) {
    return func_10(retv, pstate);
}

static inline int func_8(ast_expr_equal_t* retv, parser_state_t* pstate) {
    return func_9(retv, pstate);
}

static inline int func_7(ast_expr_equal_t* retv, parser_state_t* pstate) {
    if(NULL != (retv->expr_magnitude = parse_expr_magnitude(pstate)))
        return func_8(retv, pstate);
    else
        return STATE_NO_MATCH;
}

// begin or_function - 6
static inline int func_6(ast_expr_equal_t* retv, parser_state_t* pstate) {
    token_t* tok = get_token();
    if(tok->type == TOK_EQUAL_EQUAL || tok->type == TOK_EQU) {
        retv->tok = tok;
        return STATE_MATCH;
    }
    else
        return func_7(retv, pstate);
}

// begin grouping_function - 5
static inline int func_5(ast_expr_equal_t* retv, parser_state_t* pstate) {
    return func_6(retv, pstate);
}

// begin grouping_function - 4
static inline int func_4(ast_expr_equal_t* retv, parser_state_t* pstate) {
    return func_5(retv, pstate);
}

// begin zero_or_more_function - 3
static inline int func_3(ast_expr_equal_t* retv, parser_state_t* pstate) {
    return func_4(retv, pstate);
}

// begin grouping_function - 2
static inline int func_2(ast_expr_equal_t* retv, parser_state_t* pstate) {
    if(NULL != (retv->expr_magnitude = parse_expr_magnitude(pstate)))
        return func_3(retv, pstate);
    else
        return STATE_NO_MATCH;
}

// begin or_function - 1
static inline int func_1(ast_expr_equal_t* retv, parser_state_t* pstate) {
    return func_2(retv, pstate);
}

// begin grouping_function - 0
static inline int func_0(ast_expr_equal_t* retv, parser_state_t* pstate) {
    return func_1(retv, pstate);
}

/*
 * expr_equal (
 *      ( expr_magnitude ( ( TOK_EQUAL_EQUAL | TOK_EQU ) expr_magnitude ) * ) |
 *      ( expr_magnitude ( ( TOK_BANG_EQUAL | TOK_NEQU ) expr_magnitude ) * )
 * )
 */
ast_expr_equal_t* parse_expr_equal(parser_state_t* pstate) {

    ENTER;
    ASSERT(pstate != NULL, "null pstate is not allowed");
    ast_expr_equal_t* retv = NULL;
    int state              = 0;
    bool finished          = false;
    void* post             = mark_token_queue();

    // ast_expr_magnitude_t* expr_magnitude = NULL;
    // token_t* TERMINAL_OPER = NULL;
    // ast_expr_magnitude_t* expr_magnitude = NULL;
    // ast_expr_magnitude_t* expr_magnitude = NULL;
    // token_t* TERMINAL_OPER = NULL;
    // ast_expr_magnitude_t* expr_magnitude = NULL;

    // begin grouping_function - 0
    //   begin or_function - 1
    //     begin grouping_function - 2
    //       non-terminal rule element: expr_magnitude
    //       begin zero_or_more_function - 3
    //         begin grouping_function - 4
    //           begin grouping_function - 5
    //             begin or_function - 6
    //               terminal rule element: TOK_EQUAL_EQUAL
    //               terminal rule element: TOK_EQU
    //             end or_function
    //           end grouping_function
    //           non-terminal rule element: expr_magnitude
    //         end grouping_function
    //       end zero_or_more_function
    //     end grouping_function
    //     begin grouping_function - 7
    //       non-terminal rule element: expr_magnitude
    //       begin zero_or_more_function - 8
    //         begin grouping_function - 9
    //           begin grouping_function - 10
    //             begin or_function - 11
    //               terminal rule element: TOK_BANG_EQUAL
    //               terminal rule element: TOK_NEQU
    //             end or_function
    //           end grouping_function
    //           non-terminal rule element: expr_magnitude
    //         end grouping_function
    //       end zero_or_more_function
    //     end grouping_function
    //   end or_function
    // end grouping_function

    while(!finished) {
        switch(state) {

            // begin grouping_function - 0
            case 0:
                TRACE_STATE;
                state = 10;
                break;

            // begin or_function - 1
            case 10:
                TRACE_STATE;
                state = 20;
                break;

            // begin grouping_function - 2
            case 20:
                TRACE_STATE;
                if(NULL != (retv->expr_magnitude = parse_expr_magnitude(pstate)))
                    state = 30;
                else
                    state = STATE_NO_MATCH;
                break;

            // begin zero_or_more_function - 3
            case 30:
                TRACE_STATE;

                break;

            // begin grouping_function - 4
            case 40:
                TRACE_STATE;
                break;

            // begin grouping_function - 5
            case 50:
                TRACE_STATE;
                break;

            // begin or_function - 6
            case 60:
                TRACE_STATE;
                break;

            // begin grouping_function - 7
            case 70:
                TRACE_STATE;
                break;

            // begin zero_or_more_function - 8
            case 80:
                TRACE_STATE;
                break;

            // begin grouping_function - 9
            case 90:
                TRACE_STATE;
                break;

            // begin grouping_function - 10
            case 100:
                TRACE_STATE;
                break;

            // begin or_function - 11
            case 110:
                TRACE_STATE;
                break;

            case STATE_MATCH:
                TRACE_STATE;
                consume_token_queue();
                //retv = (ast_expr_equal_t*)create_ast_node(AST_EXPR_EQUAL);
                // retv->expr_magnitude = expr_magnitude;
                // retv->TERMINAL_OPER = TERMINAL_OPER;
                // retv->expr_magnitude = expr_magnitude;
                // retv->expr_magnitude = expr_magnitude;
                // retv->TERMINAL_OPER = TERMINAL_OPER;
                // retv->expr_magnitude = expr_magnitude;

                break;
            case STATE_NO_MATCH:
                TRACE_STATE;
                restore_token_queue(post);
                finished = true;
                break;
            case STATE_ERROR:
                TRACE_STATE;
                restore_token_queue(post);
                recover_parser_error(pstate);
                finished = true;
                break;
            default:
                FATAL("unknown state: %d", state);
        }
    }

    RETURN(&retv);
}
