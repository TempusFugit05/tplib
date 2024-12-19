#ifndef IO_H
#define IO_H

#include <cstddef>
#include "string_utils.h"

// Version of print to write string straight to stdout.
inline void print(const char* const string)
{
    str_write(string, str_length(string));
}

// Version of print with format arguments.
template <typename ...T>
inline void print(const char* const format, T... args)
{
    str_print_formatted(format, args...);
}

#endif // IO_H
