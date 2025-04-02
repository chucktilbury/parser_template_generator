
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "scanner.h"
#include "parser.h"
#include "ast.h"

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

    traverse_ast((ast_node_t*)root_node);

    return 0;
}