# Random Notes
These notes help me organize my thoughts.

Each parse function can be built up of state machine sections. A section corresponds to a function such as ``one or more``. A function has exactly one argument that can be another function. The exception to that is the ``grouping`` function which implements a list of arguments. Grouping always returns a list. Other functions return the type of object that they parse.

A return object can be a terminal or a non-terminal, so there needs to be a wrapper object to make them the same but still tell the difference when traversing them in the AST.

Each non-terminal has a single parsing function. While the parse function is being generated, the state number must be tracked so that it can be placed in a single switch/case statement. All parse functions have cases for match, no match, and error. All of the parse function also have a starting state.

## Functions
All elements in a rule are presented in functions. A rule can be considered to be a named regular expression. All functions accept exactly one argument except the grouping function which accepts a list of arguments. The ``or`` function is right-associative. The others are left-associative.

### zero-or-one
Represented by the question mark (``?``) character.
#### pattern
```
// a specific item type
ast_rule_element_t* item = NULL;
...
switch(state) {
    ...
    case XX0:
        if(NULL != (item = rule_element(pstate))) {
            consume_the_item();
            state = XX1;
        }
        else
            state = STATE_NO_MATCH;
        break;
    case XX1:
        if(token_match(TOK_QUESTION)) {
            consume_the_item();
            state = STATE_MATCH;
        }
        else
            state = STATE_NO_MATCH;
        break;
    ...
}
```

### zero-or-more
Represented by the star (``*``) character.
#### pattern
```
ptr_list_t* list = NULL;
ast_rule_element_t* item = NULL;
...
switch(state) {
    ...
    case XX0:
        if(NULL != (item = rule_element(pstate))) {
            list = create_list();
            add_item_to_list(list, item);
            consume_the_item();
            state = XX1;
        }
        else
            state = STATE_NO_MATCH;
        break;
    case XX1:
        if(NULL != (item = rule_element(pstate))) {
            add_item_to_list(list, item);
            consume_the_item();
            // no state change
        }
        else if(token_match(TOK_STAR)){
            consume_the_item();
            state = STATE_MATCH;
        }
        else
            state = STATE_NO_MATCH;
        break;
    ...
}
```

### one-or-more
Represented by the plus (``+``) character.
```
ptr_list_t* list = NULL;
ast_rule_element_t* item = NULL;
...
switch(state) {
    ...
    case XX0:
        if(NULL != (item = rule_element(pstate))) {
            list = create_list();
            add_item_to_list(list, item);
            consume_the_item();
            state = XX1;
        }
        else
            state = STATE_NO_MATCH;
        break;
    case XX1:
        if(NULL != (item = rule_element(pstate))) {
            add_item_to_list(list, item);
            consume_the_item();
            // no state change
        }
        else
            state = XX2;
        break;
    case XX2:
        if(token_match(TOK_PLUS)) {
            consume_the_item();
            state = STATE_MATCH;
        }
        else
            state = STATE_NO_MATCH;
        break;
    ...
}
```

### or
Represented by the bar (``|``) character.
```
ast_rule_element_t* item = NULL;
...
switch(state) {
    ...
    case XX:
        if(NULL != (item = rule_element(pstate))) {
            consume_the_item();
            state = XX1;
        }
        else
            state = STATE_NO_MATCH;
        break;
    case XX1:
        if(token_match(TOK_BAR)) {
            consume_the_item();
            state = STATE_MATCH;
        }
        else
            state = STATE_NO_MATCH;
        break;
    ...
}

```

### grouping
Represented by parentheses ``()``.
```
ptr_list_t* list = NULL;
ast_rule_element_t* item = NULL;
...
switch(state) {
    ...
    case XX0:
        if(token_match(TOK_OPAREN)) {
            consume_the_item();
            state = XX1;
        }
        else
            state = STATE_NO_MATCH;
        break;
    case XX1:
        if(NULL != (item = rule_element(pstate))) {
            list = create_list();
            add_item_to_list(list, item);
            consume_the_item();
            state = XX2;
        }
        else
            state = STATE_ERROR;
        break;
    case XX2:
        if(NULL != (item = rule_element(pstate))) {
            add_item_to_list(list, item);
            consume_the_item();
            // no state change
        }
        else if(token_match(TOK_CPAREN)) {
            consume_the_item();
            state = STATE_MATCH;
        }
        else
            state = STATE_ERROR;
        break;
    ...
```

### Ending States
An ending state is one where the parsing of the current non-terminal is ending and some other action will be taken afterward.
```
  ...
  case STATE_MATCH:
      // update the output data structure.
      // consume the token queue
      // return the data structure
      break;
  case STATE_NO_MATCH:
      // reset the token queue
      // return NULL
      break;
  case STATE_ERROR:
      // reset the token queue
      // execute error recovery so parsing can continue
      // return NULL
      break;
  ...
```

## Other patterns
A rule element is what the function returns but there are al lot more patterns that a rule_element is composed of. For the grammar, a rule_element looks like this:
```
rule_element (
    NON_TERMINAL |
    TERMINAL_NAME |
    TERMINAL_SYMBOL |
    TERMINAL_OPER |
    or_function |
    one_or_more_function |
    zero_or_more_function |
    zero_or_one_function |
    grouping_function
)
```
Given this definition, the entire grammar of the parser generator can be implemented. In practice, a normal grammar will be more complicated. The patterns given above will be required to parse many non-terminals so each rule will be generated using templates.
