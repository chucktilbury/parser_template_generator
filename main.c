
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "scanner.h"
#include "parser.h"
#include "ast.h"
#include "strlist.h"
#include "lists.h"

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

    int mark = 0;
    int count = 1;
    str_buf_t* sbuf;
    printf("\n");
    while(NULL != (sbuf = iterate_str_list(nterm_list, &mark)))
        printf("%3d. nterm: %s\n", count++, sbuf->buffer);

    count = 1;
    mark = 0;
    printf("\n");
    while(NULL != (sbuf = iterate_term_list(term_list, &mark)))
        printf("%3d. term: %s\n", count++, sbuf->buffer);

    mark = 0;
    count = 1;
    printf("\n");
    while(NULL != (sbuf = iterate_str_list(token_list, &mark)))
        printf("%3d. token: %s\n", count++, sbuf->buffer);

    return 0;
}