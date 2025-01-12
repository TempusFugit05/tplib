#ifndef FORMAT_H
#define FORMAT_H

#include <stdarg.h>
#include <stddef.h>

#include "string_utils.h"
#include "format_utils.h"

inline size_t str_format(char* const buffer, size_t buffer_size, const char* const format, va_list args)
{
    return str_format_internal(buffer, buffer_size, format, args);
}

template <typename ...T>
inline size_t str_format(char* const buffer, size_t buffer_size, const char* const format, T... args)
{
    va_list str_args;
    va_start(str_args, format);
    size_t ret = str_format(buffer, buffer_size, format, str_args);
    va_end(str_args);
    return ret;
}

offsets_struct format_int(const char* const format, size_t format_size, char* const buffer, const size_t buffer_size, int to_format);
size_t to_str(int integer, char* const buffer, const size_t buffer_size, const int base = 10, const bool capitalized = false, const char seperator = 0, const unsigned int seperate_every = -1);

#endif // FORMAT_H
