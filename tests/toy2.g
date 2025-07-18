translation_unit (
    translation_unit_element*
)

translation_unit_element (
    import_statement |
    data_definition |
    function_definition |
    struct_definition
    start_block
)

start_block (
    'start' function_body
)


import_statement (
    'import' STRING_LITERAL
)

data_declaration (
    type_name IDENTIFIER
)

data_definition (
    'const'? data_declaration ('=' initializer)?
)

initializer (
    expression |
    ('[' expression (',' expression)* ']') |
    ('[' dss_initializer ']') |
    ('{' dss_initializer '}')
)

dss_initializer_item (
    STRING_LITERAL ':' expression
)

dss_initializer (
    dss_initializer_item (',' dss_initializer_item)*
)

formatted_string (
    STRING_LITERAL ( '(' dss_initializer? ')' )?
)

literal_type_name (
    'int' |
    'float' |
    'string' |
    'list' |
    'dict' |
    'bool'
)

type_name (
    literal_type_name |
    compound_name
)

compound_name (
    IDENTIFIER ('.' IDENTIFIER)*
)

function_name (
    (type_name IDENTIFIER) |
    ('nothing' IDENTIFIER)
)

function_definition (
    function_name function_parameters function_body
)

function_parameters (
    : '(' (data_declaration (',' data_declaration)* )? ')'
)

struct_definition (
    'struct' IDENTIFIER '{' data_declaration (data_declaration)* '}'
)

expression (
    expression *' expression |
    expression '/' expression |
    expression '%' expression |
    expression '+' expression |
    expression '-' expression |
    expression ('>' | 'gt') expression |
    expression ('<' | 'lt') expression |
    expression ('>=' | 'gte') expression |
    expression ('<=' | 'lte') expression |
    expression ('==' | 'equ') expression |
    expression ('!=' | 'nequ') expression |
    expression ('&' | 'and') expression |
    expression ('|' | 'or') expression |
    expression '^' expression |
    ('!' | 'not') expression |
    '-' expression |
    primary_expression
)

primary_expression (
    INT_LITERAL |
    FLOAT_LITERAL |
    formatted_string |
    ('(' expression ')') |
    compound_reference
)

expression_list (
    expression (',' expression)*
)

compound_reference (
    compound_reference_element ('.' compound_reference_element)*
)

compound_reference_element (
    IDENTIFIER |
    function_reference |
    list_reference
)

function_reference (
    IDENTIFIER '(' expression_list ')'
)

list_reference (
    IDENTIFIER '[' expression ']' ('[' expression ']')*
)

function_body_element (
    assignment |
    compound_reference |
    data_definition |
    struct_definition |
    if_clause |
    while_clause |
    do_clause |
    for_clause |
    return_statement |
    exit_statement |
    INLINE
)

function_body_prelist (
    function_body_element |
    function_body
)

function_body_list (
    function_body_prelist function_body_prelist*
)

function_body (
    '{' function_body_list '}'
)

loop_body_element (
    function_body_element |
    CONTINUE |
    BREAK
)


loop_body_prelist (
    loop_body_element |
    loop_body
)

loop_body_list (
    loop_body_prelist loop_body_prelist*
)

loop_body (
    '{' loop_body_list '}'
)

assignment (
    compound_name '=' expression
)

if_clause (
    'if' '(' expression ')' function_body else_clause* final_else_clause?
)

else_clause (
    'else' '(' expression ')' function_body
)

final_else_clause (
    'else' ('(' ')')? function_body
)

while_clause (
    'while' ('(' expression? ')')? loop_body
)

do_clause (
    'do' loop_body 'while' ('(' expression? ')')?
)

for_clause (
    'for' ('(' (literal_type_name? IDENTIFIER 'in')? expression ')')? loop_body
)

return_statement (
    'return' ('(' expression? ')')?
)

exit_statement (
    'exit' '(' expression ')'
)
