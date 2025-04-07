%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "render.h"

char template_buffer[1024];

%}

%option yylineno
%option noinput
%option nounput
%option noyywrap
    //%option header-file="template.h"
    //%option outfile="template.c"
%option prefix="template_"

%x NAME

%%

"{{" {
    memset(template_buffer, 0, sizeof(template_buffer));
    BEGIN(NAME);
}

<NAME>[[:alpha:][:digit:]]+ {
    strncpy(template_buffer, template_text, sizeof(template_buffer)-1);
    return 500;
}

<NAME>[ \n\r\v\f\t]* { /* drop the spaces */ }

<NAME>"}}" {
    BEGIN(INITIAL);
}

<NAME>"{{" {
    fprintf(stderr, "\nerror line %d: nested markers are not supported\n", template_lineno);
    fprintf(template_out, "\n{{ error line %d: nested markers are not supported }}\n", template_lineno);
    exit(1);
}

"}}" {
    fprintf(stderr, "\nerror line %d: misplaced marker\n", template_lineno);
    fprintf(template_out, "\n{{ error line %d: misplaced marker }}\n", template_lineno);
    exit(1);
}

[\n\r]|[:print:]* {
    fprintf(template_out, "%s",template_text);
}

%%