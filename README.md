# pgen
Recursive decent parser generator for C

This is a parser generator that generates a recursive decent parser. It is a work in progress.

The generated parser simply generates the AST and the code to traverse it. The person using it will use the traverse functions to implement whatever they are trying to do with the parser.

Flex and Bison are required to build, but nothing else is needed. Just type ``make`` to build it. There is a sample grammar in the repo as well.
