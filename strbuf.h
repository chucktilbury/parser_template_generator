#ifndef _STRBUF_H_
#define _STRBUF_H_

#include <stddef.h>

typedef struct _string_buf_t_ {
    char* buffer;
    size_t len;
    size_t cap;
} str_buf_t;

str_buf_t* create_string_buf(const char* str);
str_buf_t* create_string_buf_fmt(const char* fmt, ...);
void destroy_string_buf(str_buf_t* buf);
void append_string_buf(str_buf_t* buf, const char* str);
void append_string_buf_fmt(str_buf_t* buf, const char* fmt, ...);
void append_string_buf_char(str_buf_t* buf, int ch);
const char* raw_string_buf(str_buf_t* buf);
int len_string_buf(str_buf_t* buf);
int comp_string_buf(str_buf_t* buf1, str_buf_t* buf2);

void strip_quotes(str_buf_t* buf);
void strip_space(str_buf_t* buf);

#endif /* _STRBUF_H_ */
