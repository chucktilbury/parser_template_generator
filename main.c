
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "scanner.h"
#include "parser.h"
#include "ast.h"
#include "strlist.h"
#include "term_list.h"
#include "nterm_list.h"
#include "lists.h"

extern nterm_list_t* nterm_list;
extern term_list_t* term_list;

int main(int argc, char** argv) {

    if(argc < 2) {
        fprintf(stderr, "%s: filename\n", argv[0]);
        exit(1);
    }

    yyin = fopen(argv[1], "r");
    if(yyin == NULL) {
        fprintf(stderr, "cannot open input file \"%s\": %s", argv[1], strerror(errno));
        exit(1);
    }

    yyparse();

    make_raw_lists(root_node);

    int mark  = 0;
    int count = 1;
    printf("\n");
    term_item_t* term;
    while(NULL != (term = iterate_term_list(term_list, &mark)))
        printf("%3d. %-15s%s\n", count++, raw_string(term->term), raw_string(term->token));

    mark  = 0;
    count = 1;
    printf("\n");
    nterm_item_t* nterm;
    while(NULL != (nterm = iterate_nterm_list(nterm_list, &mark)))
        printf("%3d. %s\n", count++, raw_string(nterm->nterm));

    return 0;
}