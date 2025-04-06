#ifndef _STRBUF_H_
#define _STRBUF_H_

#include <stddef.h>

typedef const char* string_t;

string_t create_string(const char* str);
string_t create_string_fmt(const char* fmt, ...);
void destroy_string(string_t buf);
string_t append_string(string_t buf, const char* str);
string_t append_string_fmt(string_t buf, const char* fmt, ...);
string_t append_string_char(string_t buf, int ch);
int len_string(string_t buf);
int comp_string(string_t buf1, string_t buf2);

string_t strip_quotes(string_t buf);
string_t upcase(string_t buf);
string_t strip_space(string_t buf);
string_t convert(const char* str);
string_t copy_string(string_t buf);

#endif /* _STRBUF_H_ */
