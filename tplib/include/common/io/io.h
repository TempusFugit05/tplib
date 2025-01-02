#ifndef IO_H
#define IO_H

#include "string_utils.h"
#include "system_files.h"
#include "string.h"

void str_write(const char* const string, const size_t string_size, const int file);
void str_write(const char* const string, const int file);

// Version of print to write string straight to stdout.
inline void print(const char* const string)
{
    str_write(string, str_length(string), FILE_STDOUT);
}

// Version of print with format arguments.
template <typename ...T>
inline void print(const char* const format, T... args)
{
    str_print_formatted(format, args...);
}

#endif // IO_H
