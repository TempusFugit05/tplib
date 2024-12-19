#ifndef STR_UTILS_H
#define STR_UTILS_H

#include "system_params.h"
#include <cstddef>

size_t str_length(const char* const string);
void str_write(const char* const string, const size_t string_size, const int file = STDOUT_FILE);
void str_write(const char* const string, const int file = STDOUT_FILE);


void str_print(const char* const format);
void str_print_formatted(const char* const format, ...);

#endif // STR_UTILS_H
