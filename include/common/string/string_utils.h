#ifndef STR_UTILS_H
#define STR_UTILS_H

#include "system_params.h"
#include <cstddef>

#define BASE_MAX 36

const char capital_digits[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
const char lower_digits[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};

struct offsets_struct
{
    size_t buffer; // Number of chars written to the buffer.
    size_t format; // Number of chars read from the format.
};

void str_print(const char* const format);
void str_print_formatted(const char* const format, ...);

#endif // STR_UTILS_H
