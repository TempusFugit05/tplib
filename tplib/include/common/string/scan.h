#ifndef SCAN_H
#define SCAN_H

#include <cstddef>

typedef struct str_num_struct
{
    int number;
    int status;
    int num_digits;
} str_num_struct;

// Turn a string of digits to a number.
// search_whole - search the whole string for a number, else search only from the start of the string.
// status - -1 if no number found, otherwise offset from the start of the string where a number was found.
// num_digits - Number of digits found in the number returned.
str_num_struct str_to_num(const char* str, size_t str_size, const bool search_whole = false,  unsigned int base = 11, bool capitalized = false);
#endif // SCAN_H
