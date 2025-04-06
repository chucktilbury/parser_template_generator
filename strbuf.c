/**
 * @file strbuf.c
 *
 * @brief
 *
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.1
 * @date 2025-03-25
 * @copyright Copyright (c) 2025
 */
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#include "alloc.h"
#include "strbuf.h"

string_t create_string(const char* str) {

    return _COPY_STRING(str);
}

string_t create_string_fmt(const char* fmt, ...) {

    va_list args;

    va_start(args, fmt);
    size_t len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    char* str = _ALLOC(len+1);

    va_start(args, fmt);
    vsprintf(str, fmt, args);
    va_end(args);

    return str;
}

void destroy_string(string_t buf) {

    if(buf != NULL) {
        _FREE(buf);
    }
}

string_t append_string(string_t buf, const char* str) {

    size_t len1 = strlen(str);
    size_t len2 = strlen(buf);
    char* ptr = _ALLOC(len1 + len2 + 1);

    strcpy(ptr, buf);
    strcat(ptr, str);

    return ptr;
}

string_t append_string_fmt(string_t buf, const char* fmt, ...) {

    va_list args;

    va_start(args, fmt);
    size_t len1 = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    size_t len2 = strlen(buf);
    char* str = _ALLOC(len1 + len2 + 1);
    strcpy(str, buf);

    va_start(args, fmt);
    vsprintf(&str[len2], fmt, args);
    va_end(args);

    return str;
}

string_t append_string_char(string_t buf, int ch) {

    size_t len = strlen(buf);
    char* str = _ALLOC(len + 2);

    strcpy(str, buf);
    str[len] = (char)ch;
    str[len+1] = '\0';

    return str;
}

int len_string(string_t buf) {

    return strlen(buf);
}

int comp_string(string_t buf1, string_t buf2) {

    return strcmp(buf1, buf2);
}

string_t strip_quotes(string_t buf) {

    strip_space(buf);

    char* temp = strrchr(buf, '\"');
    if(temp != NULL)
        *temp = '\0';
    else if(NULL != (temp = strrchr(buf, '\'')))
        *temp = '\0';

    if(buf[0] == '\'' || buf[0] == '\"')
        memmove(&((char*)buf)[0], &buf[1], strlen(buf));

    return buf;
}

string_t strip_space(string_t buf) {

    size_t len;
    for(len=strlen(buf); (isspace(buf[len])||buf[len]==0)&&len>= 0; len--)
        ((char*)buf)[len] = '\0';

    for(len=0; buf[len]!='\0'&&isspace(buf[len]); len++) {}

    memmove(&((char*)buf)[0], &buf[len], strlen(&buf[len])+1);

    return buf;
}

string_t upcase(string_t buf) {

    for(int i = 0; buf[i] != '\0'; i++)
        ((char*)buf)[i] = toupper(buf[i]);

    return buf;
}

string_t convert(const char* str) {

    string_t buf = create_string(NULL);

    for(int i = 0; str[i] != '\0'; i++) {
        switch(str[i]) {
            case '~': buf = append_string(buf, "TILDE"); break;
            case '`': buf = append_string(buf, "BQUOTE"); break;
            case '!': buf = append_string(buf, "BANG"); break;
            case '@': buf = append_string(buf, "AT"); break;
            case '#': buf = append_string(buf, "POUND"); break;
            case '$': buf = append_string(buf, "DOLLAR"); break;
            case '%': buf = append_string(buf, "PERCENT"); break;
            case '^': buf = append_string(buf, "CARET"); break;
            case '&': buf = append_string(buf, "AMP"); break;
            case '*': buf = append_string(buf, "STAR"); break;
            case '(': buf = append_string(buf, "OPAREN"); break;
            case ')': buf = append_string(buf, "CPAREN"); break;
            case '-': buf = append_string(buf, "MINUS"); break;
            case '=': buf = append_string(buf, "EQUAL"); break;
            case '+': buf = append_string(buf, "PLUS"); break;
            case '[': buf = append_string(buf, "OSBRACE"); break;
            case '{': buf = append_string(buf, "OCBRACE"); break;
            case ']': buf = append_string(buf, "CSBRACE"); break;
            case '}': buf = append_string(buf, "CCBRACE"); break;
            case '\\': buf = append_string(buf, "BSLASH"); break;
            case '|': buf = append_string(buf, "BAR"); break;
            case ';': buf = append_string(buf, "SCOLON"); break;
            case ':': buf = append_string(buf, "COLON"); break;
            case '\'': buf = append_string(buf, "SQUOTE"); break;
            case '\"': buf = append_string(buf, "DQUOTE"); break;
            case ',': buf = append_string(buf, "COMMA"); break;
            case '<': buf = append_string(buf, "OPBRACE"); break;
            case '.': buf = append_string(buf, "DOT"); break;
            case '>': buf = append_string(buf, "CPBRACE"); break;
            case '/': buf = append_string(buf, "SLASH"); break;
            case '?': buf = append_string(buf, "QUESTION"); break;
        }
        if(str[i+1] != '\0')
            buf = append_string_char(buf, '_');
    }

    return buf;
}

string_t copy_string(string_t buf) {

    return _COPY_STRING(buf);
}

#if 0
int main(void) {

    string_t ptr = create_string("'   this isi the string   \"");

    strip_quotes(ptr);
    strip_space(ptr);

    printf("\"%s\"\n", ptr->buffer);

    append_string_fmt(ptr, " carp: %d and %s", 24, "another string");

    printf("\"%s\"\n", ptr->buffer);

    return 0;
}
#endif
