# pgen
Recursive decent parser generator for C

This is a parser generator that generates a recursive decent parser. It is a work in progress. The goal it to create a working parser and AST generator/traverse from a simplified grammar. A scanner is not generated (yet) and there are specific rules to relate to the case and placement of symbols in the grammar.

* A ``[a-z_][a-z0-9_]*`` is a non terminal symbol. It is defined exactly once and then may be referenced any number of times.
* A ``[A-Z_][A-Z0-9_]*`` is a terminal that must be constructed in the scanner. So when the parser generator encounters a token that looks like ``WOMBAT99`` it will create a token type of ``TOK_WOMBAT99`` and assign a number to it and the generated parser will expect to see a ``TOK_WOMBAT99`` in the input stream. The scanner for the generated parser will return that as a token type when it scans whatever a ``TOK_WOMBAT99`` is supposed to represent.
* A ``\'[a-zA-Z_][a-zA-Z0-9_]*\'`` is a keyword definition such as ``'while'`` or ``'import'``. A token type is created such as ``TOK_WHILE`` and assigned a number. The generated parser refers to it by that symbolic designation.
* A ``\'[^a-zA-Z0-9_\']+\'`` is an operator. For example the string ``'!='`` translates to ``TOK_BANG_EQUAL`` in the generated parser. The scanner for the generated parser must recognize that string and return the token type.
* Comments start with a ``#`` character and continue to the end of the line.
* All non-printable characters are ignored.
* See ``pgen/src/parser/scanner.l`` for the complete scanner rules.

The generated parser simply generates the AST and the code to traverse it. The person using it will use the traverse functions to implement whatever they are trying to do with the parser.

## Requirements

* flex and bison
* cmake
* doxygen
* graphviz (for doxygen)
* Any ANSI C compiler. I am currently using clang 18.
* git (of course)

## Building

* Change to the ``pgen`` directory and type ``./scripts/setup`` That will create the build directory and some others for the build, such as adding the binary output directory to ``$PATH``. It also sets the path and some other stuff. to know more, check out ``./pgen/.project_env``. It's a shell script that is run **after** the new shell starts up. To exit the build environment just type ``exit``.

* Just type ``make`` to build the code.

* To test it, try one of the test grammars, such as ``pgen ../test/toy.g``. Then look at ``./out/ast/ast.h`` and see what was generated. There are a few files generated to the ``out`` directory.

## Input Grammar.

See below for the full grammer of the input grammer written in the form of the input grammar.

The input grammar is simply a list of labeled regular expressions called rules. These expressions are very limited and simple. The label of the expression is the non-terminal symbol and the expression is the rule associated with it. All of these expressions start with a grouping. Thus a complete rule may look something like ``rule_name ( rule_elements )``. The symbol ``rule_elements`` refers to another rule defined in this grammar.

### Input Grammar
See ``pgen/src/parser/parser.y`` for the Bison implementation of this grammar.

* ``+`` means "one or more".
* ``*`` means "zero or more".
* ``?`` means "one or zero".
* ``|`` means "or".
* ``()`` means "grouping".

#### Complete listing
```
# This is a grammar for the input to pgen.

grammar (
    rule+
)

rule (
    NON_TERMINAL grouping_function+
)

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

or_function (
    rule_element PIPE rule_element
)

one_or_more_function (
    rule_element PLUS
)

zero_or_more_function (
    rule_element STAR
)

zero_or_one_function (
    rule_element QUESTION
)

grouping_function (
    OPAREN rule_element+ CPAREN
)

```

