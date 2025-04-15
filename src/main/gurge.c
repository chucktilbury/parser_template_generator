/**
 * @file gurge.c
 *
 * @brief AST pass that prints the regurgitates the input after processing.
 *
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.1
 * @date 2025-04-01
 * @copyright Copyright (c) 2025
 */

#include "ast.h"
#include "parser.h"
#include "lists.h"

#define USE_TRACE
#include "common.h"

#ifdef USE_TRACE
#define TRACE_TOKEN(t) TRACE("token: \"%s\": %s", raw_string((t)->str), tok_to_str((t)->type))
#else
#define TRACE_TOKEN(t)
#endif

static void grammar(grammar_t* node);
static void grammar_list(grammar_list_t* node);
static void grammar_rule(grammar_rule_t* node);
static void rule_element_list(rule_element_list_t* node);
static void rule_element(rule_element_t* node);
static void or_function(or_function_t* node);
static void zero_or_more_function(zero_or_more_function_t* node);
static void zero_or_one_function(zero_or_one_function_t* node);
static void one_or_more_function(one_or_more_function_t* node);
static void grouping_function(grouping_function_t* node);

/*
 grammar
 : grammar_list
 ;
 */
static void grammar(grammar_t* node) {

    ENTER;

    grammar_list(node->grammar_list);

    RETURN();
}


/*
 grammar_list
 : non_terminal_rule  SEMI
 | grammar non_terminal_rule SEMI
 ;
 */
static void grammar_list(grammar_list_t* node) {

    ENTER;
    int mark = 0;
    grammar_rule_t* rule;

    while(NULL != (rule = (grammar_rule_t*)iterate_ast_node_list(node->list, &mark)))
        grammar_rule(rule);

    RETURN();
}


/*
grammar_rule
 : NON_TERMINAL grouping_function
 ;
 */
static void grammar_rule(grammar_rule_t* node) {

    ENTER;

    TRACE_TOKEN(node->NON_TERMINAL);
    printf("\n%s ", node->NON_TERMINAL->str->buffer);
    grouping_function(node->grouping_function);
    printf("\n");

    RETURN();
}


/*
rule_element_list
 : rule_element
 | rule_element_list rule_element
 ;
 */
static void rule_element_list(rule_element_list_t* node) {

    ENTER;
    int mark = 0;
    rule_element_t* rule;

    while(NULL != (rule = (rule_element_t*)iterate_ast_node_list(node->list, &mark)))
        rule_element(rule);

    RETURN();
}


/*
rule_element
 : NON_TERMINAL
 | TERMINAL_NAME
 | TERMINAL_OPER
 | TERMINAL_SYMBOL
 | or_function
 | zero_or_more_function
 | zero_or_one_function
 | one_or_more_function
 | grouping_function
 ;
 */
static void rule_element(rule_element_t* node) {

    ENTER;

    if(node->token != NULL) {
        TRACE_TOKEN(node->token);
        switch(node->token->type) {
            case TERMINAL_OPER:
            case TERMINAL_NAME: {
                    term_item_t* term = find_term(master_list->term_list, node->token->str->buffer);
                    printf("%s ", term->token->buffer);
                }
                break;
            case TERMINAL_SYMBOL:
            case NON_TERMINAL:
                printf("%s ", node->token->str->buffer);
                break;
            default:
                FATAL("unknown terminal type: %s", tok_to_str(node->token->type));
        }
    }
    else if(node->nterm != NULL) {
        switch(node->nterm->type) {
            case AST_OR_FUNCTION:
                or_function((or_function_t*)node->nterm);
                break;
            case AST_ZERO_OR_MORE_FUNCTION:
                zero_or_more_function((zero_or_more_function_t*)node->nterm);
                break;
            case AST_ZERO_OR_ONE_FUNCTION:
                zero_or_one_function((zero_or_one_function_t*)node->nterm);
                break;
            case AST_ONE_OR_MORE_FUNCTION:
                one_or_more_function((one_or_more_function_t*)node->nterm);
                break;
            case AST_GROUPING_FUNCTION:
                grouping_function((grouping_function_t*)node->nterm);
                break;
            default:
                FATAL("unknown non-terminal type: %s", nterm_to_str(node->nterm->type));
        }
    }
    else
        FATAL("invalid rule element");

    RETURN();
}

/*
or_function
 : rule_element PIPE rule_element
 ;
 */
static void or_function(or_function_t* node) {

    ENTER;

    rule_element(node->rule_element);
    printf("| ");

    RETURN();
}

/*
zero_or_more_function
 : rule_element QUESTION
 ;
 */
static void zero_or_more_function(zero_or_more_function_t* node) {

    ENTER;

    rule_element(node->rule_element);
    printf("* ");

    RETURN();
}

/*
zero_or_one_function
 : rule_element STAR
 ;
 */
static void zero_or_one_function(zero_or_one_function_t* node) {

    ENTER;

    rule_element(node->rule_element);
    printf("? ");

    RETURN();
}


/*
one_or_more_function
 : rule_element PLUS
 ;
 */
static void one_or_more_function(one_or_more_function_t* node) {

    ENTER;

    rule_element(node->rule_element);
    printf("+ ");

    RETURN();
}

/*
grouping_function
 : OPAREN rule_element_list CPAREN
 ;
 */
static void grouping_function(grouping_function_t* node) {

    ENTER;

    printf("( ");
    rule_element_list(node->rule_element_list);
    printf(") ");

    RETURN();
}

void regurgitate_ast(grammar_t* node) {

    grammar(node);
}

