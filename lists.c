/**
 * @file lists.c
 *
 * @brief Traverse the AST to create the base lists. Terminals and
 * nonterminals as well as their associated objects.
 *
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.1
 * @date 2025-04-01
 * @copyright Copyright (c) 2025
 */

#include "ast.h"
#include "errors.h"
#include "lists.h"
#include "parser.h"
#include "alloc.h"

#include "nterm_list.h"
#include "term_list.h"
// global product produced by this file
nterm_list_t* nterm_list;
term_list_t* term_list;

#define USE_TRACE
#include "trace.h"

#ifdef USE_TRACE
#define TRACE_TOKEN(t) TRACE("token: \"%s\": %s", (t)->str, tok_to_str((t)->type))
#else
#define TRACE_TOKEN(t)
#endif

static void grammar(grammar_t* node);
static void grammar_list(grammar_list_t* node);
static void non_terminal_rule(non_terminal_rule_t* node);
static void rule_element_list(rule_element_list_t* node);
static void rule_element(rule_element_t* node);
static void terminal_rule_element(terminal_rule_element_t* node);
static void non_terminal_rule_element(non_terminal_rule_element_t* node);
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
    non_terminal_rule_t* rule;

    while(NULL != (rule = (non_terminal_rule_t*)iterate_ast_node_list(node->list, &mark)))
        non_terminal_rule(rule);

    RETURN();
}


/*
non_terminal_rule
 : NON_TERMINAL COLON rule_element_list
 ;
 */
static void non_terminal_rule(non_terminal_rule_t* node) {

    ENTER;

    TRACE_TOKEN(node->NON_TERMINAL);

    append_nterm_list(nterm_list, create_nterm_item(node->NON_TERMINAL->str));
    rule_element_list(node->rule_element_list);

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
 : terminal_rule_element
 | non_terminal_rule_element
 ;
 */
static void rule_element(rule_element_t* node) {

    ENTER;

    switch(node->nterm->type) {
        case AST_TERMINAL_RULE_ELEMENT:
            terminal_rule_element((terminal_rule_element_t*)node->nterm);
            break;
        case AST_NON_TERMINAL_RULE_ELEMENT:
            non_terminal_rule_element((non_terminal_rule_element_t*)node->nterm);
            break;
        default:
            FATAL("unknown node type: %d", node->nterm->type);
    }

    RETURN();
}

/*
terminal_rule_element
 : NON_TERMINAL
 | TERMINAL_NAME
 | TERMINAL_OPER
 | TERMINAL_SYMBOL
 ;
 */
static void terminal_rule_element(terminal_rule_element_t* node) {

    ENTER;

    TRACE_TOKEN(node->token);

    switch(node->token->type) {
        case TERMINAL_NAME: {
                string_t term = create_string(node->token->str);
                strip_quotes(term);

                string_t tok = copy_string(term);
                upcase(tok);
                tok = create_string_fmt("TOK_%s", tok);

                append_term_list(term_list, create_term_item(term, tok));

                destroy_string(term);
                destroy_string(tok);
            }
            break;
        case TERMINAL_OPER: {
                string_t term = create_string(node->token->str);
                strip_quotes(term);

                string_t tok = copy_string(term);
                tok = convert(tok);
                tok = create_string_fmt("TOK_%s", tok);

                append_term_list(term_list, create_term_item(term, tok));

                destroy_string(term);
                destroy_string(tok);
            }
            break;
        case TERMINAL_SYMBOL: {
                string_t term = copy_string(node->token->str);
                string_t tok = create_string_fmt("TOK_%s", node->token->str);

                append_term_list(term_list, create_term_item(term, tok));

                destroy_string(term);
                destroy_string(tok);
            }
            break;
        case NON_TERMINAL:
            /* do nothing */
            break;
        default:
            FATAL("invalid token type: %s", tok_to_str(node->token->type));
    }

    RETURN();
}

/*
non_terminal_rule_element
 : or_function
 | zero_or_more_function
 | zero_or_one_function
 | one_or_more_function
 | grouping_function
 ;
 */
static void non_terminal_rule_element(non_terminal_rule_element_t* node) {

    ENTER;

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
            FATAL("unknown node type: %d", node->nterm->type);
    }

    RETURN();
}

/*
or_function
 : rule_element PIPE
 ;
 */
static void or_function(or_function_t* node) {

    ENTER;

    rule_element(node->left);
    rule_element(node->right);

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

    RETURN();
}

/*
grouping_function
 : OPAREN rule_element_list CPAREN
 ;
 */
static void grouping_function(grouping_function_t* node) {

    ENTER;

    rule_element_list(node->rule_element_list);

    RETURN();
}

/**
 * @brief Create all of the basic lists.
 *
 * @param node
 */
void make_raw_lists(grammar_t* node) {

    nterm_list = create_nterm_list();
    term_list = create_term_list();

    grammar((grammar_t*)node);
}


