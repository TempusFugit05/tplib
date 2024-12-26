#ifndef FORMAT_H
#define FORMAT_H
#include <stdarg.h>

size_t str_format(const char* const format, char* const buffer, size_t buffer_size, va_list args);
size_t to_str(int integer, char* const buffer, const size_t buffer_size, const int base = 10, const bool capitalized = false, const bool big_endian = true, const char seperator = 0, const unsigned int seperate_every = -1);

#endif // FORMAT_H
