
# This is a grammar for the simple language.
# Note that this is very old and inacurate.

module (
    module_item+
)

module_item (
    namespace_item |
    import_statement |
    include_statement |
    start_definition
)

start_definition (
    'start' function_body
)

import_statement (
    'import' formatted_strg 'as' IDENT
)

include_statement (
    'include' formatted_strg
)

alias_definition (
    compound_name 'as' IDENT
)

namespace_item (
    scope_operator |
    namespace_definition |
    class_definition |
    function_definition |
    create_definition |
    destroy_definition |
    var_definition |
    alias_definition
)

scope_operator (
    'private' |
    'public' |
    'protected'
)

literal_type_name (
    'float' |
    'integer' |
    'string' |
    'boolean' |
    'nothing' |
    'list' |
    'dict'
)

type_name (
    literal_type_name | compound_name
)

formatted_strg (
    LITERAL_DSTR ( expression_list )?
)

string_literal (
    LITERAL_SSTR | formatted_strg
)

literal_value (
    LITERAL_FLOAT |
    LITERAL_INTEGER |
    LITERAL_BOOL |
    string_literal
)

var_decl (
    type_name IDENT
)

func_parm_decl (
    type_name ( IDENT )?
)

func_parm_decl_list (
    '(' ( func_parm_decl ( ',' func_parm_decl )* )? ')'
)

assignment_item (
    expression | list_init
)

var_definition (
    ( 'const' )? var_decl ( '=' assignment_item )?
)

list_init_str (
    LITERAL_DSTR | LITERAL_SSTR
)

list_init_element (
    list_init_str ':' assignment_item |
    assignment_item
)

list_init (
    '[' list_init_element ( ',' list_init_element )* ']'
)

array_param_item (
    expression | string_literal
)

array_param (
    '[' array_param_item ']'
)

array_param_list (
    array_param (array_param)*
)

array_reference (
    IDENT array_param_list
)

function_reference (
    compound_name expression_list compound_name_list
)

create_reference (
    IDENT ( '.' IDENT )* '.' 'create' expression_list
)

destroy_reference (
    IDENT ( '.' IDENT )* '.' 'destroy'
)

compound_name (
    IDENT ( '.' IDENT )*
)

compound_name_list (
    '(' ( compound_name (',' compound_name )* )? ')'
)

compound_ref_item (
    IDENT | array_reference
)

compound_reference (
    compound_ref_item ( '.' compound_ref_item )*
)

cast_statement (
    type_name ':' expression
)

expression (
    expr_and ( 'or' expr_and )*
)

expr_and (
    expr_equality ( 'and' expr_equality )*
)

expr_equality (
    expr_compare ( '==' expr_compare )* |
    expr_compare ( '!=' expr_compare )*
)

expr_compare (
    expr_term ( '<' expr_term )* |
    expr_term ( '>' expr_term )* |
    expr_term ( '<=' expr_term )* |
    expr_term ( '>=' expr_term )*
)

expr_term (
    expr_factor ( '+' expr_factor )* |
    expr_factor ( '-' expr_factor )*
)

expr_factor (
    expr_unary ( '*' expr_unary )* |
    expr_unary ( '/' expr_unary )* |
    expr_unary ( '%' expr_unary )*
)

expr_unary (
    '-' expr_primary* |
    '!' expr_primary*
)

expr_primary (
    literal_value |
    compound_reference |
    cast_statement |
    '(' expression ')'
)

expression_list (
    '(' (expression ( ',' expression )*)? ')'
)

namespace_definition (
    'namespace' IDENT '{' ( namespace_item )+ '}'
)

class_inheritance_item (
    (scope_operator)? compound_name 'as' IDENT
)

class_inheritance_list (
    '(' ( class_inheritance_item (',' class_inheritance_item)* )? ')'
)

class_definition (
    'class' IDENT ( class_inheritance_list )? class_body
)

class_body_item (
    scope_operator |
    var_decl |
    function_definition |
    create_definition |
    destroy_definition
)

class_body (
    '{' ( class_body_item )+ '}'
)

function_membership (
    compound_name ':'
)

function_definition (
    ( function_membership )? IDENT func_parm_decl_list func_parm_decl_list ( function_body )?
)

create_definition (
    ( function_membership )? 'create' func_parm_decl_list ( function_body )?
)

destroy_definition (
    ( function_membership )? 'destroy' ( function_body )?
)

function_body (
    '{' ( function_body_element )* '}'
)

loop_body (
    '{' ( loop_body_element )* '}'
)

assign_eq_item (
    assignment_item | compound_reference
)

assign_inc_item (
    expression | string_literal
)

assignment (
    compound_reference '=' assign_eq_item |
    compound_reference '+=' assign_inc_item |
    compound_reference '-=' expression |
    compound_reference '*=' expression |
    compound_reference '/=' expression |
    compound_reference '%=' expression
)

function_body_element (
    var_definition |
    function_reference |
    create_reference |
    destroy_reference |
    assignment |
    while_clause |
    do_clause |
    for_clause |
    if_clause |
    try_clause |
    INLINE |
    type_statement |
    return_statement |
    raise_statement |
    trace_statement |
    print_statement |
    exit_statement |
    function_body
)

loop_body_element (
    function_body_element |
    break_statement |
    continue_statement |
    yield_statement |
    loop_body
)


break_statement (
    'break'
)

continue_statement (
    'continue'
)

yield_statement (
    'yield' '(' compound_reference ')'
)

type_statement (
    'type' '(' compound_reference ')'
)

return_statement (
    'return'
)

raise_statement (
    'raise' '(' IDENT ',' formatted_strg ')'
)

trace_statement (
    'trace' '(' string_literal ')'
)

print_statement (
    'print' ( expression_list )?
)

exit_statement (
    'exit'
)

while_definition (
    'while' ( '(' ( expression )? ')' )?
)

while_clause (
    while_definition loop_body
)

do_clause (
    'do' function_body while_definition
)

for_clause (
    'for' ( '(' ( expression 'as' IDENT )? ')' )? loop_body
)

if_clause (
    'if' '(' expression ')' function_body ( else_clause )* ( final_else_clause )?
)

else_clause (
    'else' '(' expression ')' function_body
)

final_else_clause (
    'else' ( '(' ')' )? function_body
)

try_clause (
    'try' function_body ( except_clause )* ( final_clause )?
)

except_clause (
    'except' '(' IDENT ',' IDENT ')' function_body
)

final_clause (
    'final' '(' IDENT ')' function_body
)

