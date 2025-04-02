/**
 * @file tokens.c
 *
 * @brief
 *
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.1
 * @date 2025-04-01
 * @copyright Copyright (c) 2025
 */

#include "alloc.h"
#include "tokens.h"
#include "parser.h"

token_t* create_token(const char* str, int type) {

    token_t* tok = _ALLOC_TYPE(token_t);
    tok->type = type;
    tok->str = _COPY_STRING(str);

    return tok;
}

const char* tok_to_str(int type) {

    return (type == PLUS)? "PLUS" :
        (type == STAR)? "STAR" :
        (type == QUESTION)? "QUESTION" :
        (type == PIPE)? "PIPE" :
        (type == COLON)? "COLON" :
        (type == SEMI)? "SEMI" :
        (type == OPAREN)? "OPAREN" :
        (type == CPAREN)? "CPAREN" :
        (type == NON_TERMINAL)? "NON_TERMINAL" :
        (type == TERMINAL_SYMBOL)? "TERMINAL_SYMBOL" :
        (type == TERMINAL_NAME)? "TERMINAL_NAME" :
        (type == TERMINAL_OPER)? "TERMINAL_OPER" : "UNKNOWN";

}