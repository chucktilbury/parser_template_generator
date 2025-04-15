
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "scanner.h"
#include "parser.h"
#include "ast.h"
#include "common.h"
#include "lists.h"
#include "render.h"

void regurgitate_ast(grammar_t* node);
extern master_list_t* master_list;

void print_terminal_list(void) {

    int mark  = 0;
    int count = 1;

    printf("\n-----------------------------\n");
    printf("     Terminal List\n");
    printf("-----------------------------\n");

    term_item_t* term;
    while(NULL != (term = iterate_term_list(master_list->term_list, &mark)))
        printf("%3d. %-25s%s\n", count++, raw_string(term->term), raw_string(term->token));
}

void print_nonterminal_list(void) {

    int mark  = 0;
    int count = 1;

    printf("\n-----------------------------\n");
    printf("     Non-terminal List\n");
    printf("-----------------------------\n");
    nterm_item_t* nterm;
    while(NULL != (nterm = iterate_nterm_list(master_list->nterm_list, &mark)))
        printf("%3d. %-25s%s\n", count++, raw_string(nterm->nterm), raw_string(nterm->type));
}

int main(int argc, char** argv) {

    if(argc < 2) {
        fprintf(stderr, "Use: %s filename\n", argv[0]);
        exit(1);
    }

    yyin = fopen(argv[1], "r");
    if(yyin == NULL) {
        fprintf(stderr, "cannot open input file \"%s\": %s", argv[1], strerror(errno));
        exit(1);
    }

    yyparse();

    make_raw_lists(root_node);

    // print_nonterminal_list();
    // print_terminal_list();

    render();
    regurgitate_ast(root_node);

    return 0;
}


