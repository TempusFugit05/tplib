#include <cstddef>
#include <stdarg.h>

#include "tp_string.h"
#include "macro_utils.h"
#include "dev_debug.h"

// Get a digit from a character.
int char_to_digit(const char character, bool capitalized)
{
    const char* const digits = (capitalized) ? capital_digits : lower_digits;
    for(size_t i = 0; i < ARRAY_LENGTH(((capitalized) ? capital_digits : lower_digits)); i++)
    {
        if(character == digits[i])
        {
            return i;
        }
    }
    return 0;
}

// Copy a string to a buffer
// Returns number of characters copied
size_t str_copy(const char* const to_copy, const size_t to_copy_size, char* const copy_to, const size_t copy_to_size)
{
    size_t chars_to_copy = str_length(to_copy);
    chars_to_copy = (to_copy_size > chars_to_copy) ? chars_to_copy : to_copy_size;
    size_t i = 0;
    for(; i < chars_to_copy && i < copy_to_size && to_copy[i] != '\0'; i++)
    {
        copy_to[i] = to_copy[i];
    }
    return i; // Return number of chars written
}

// Check if a character is a digit.
bool is_digit(const char character, unsigned int base, bool capitalized)
{
    DEV_ASSERT(base <= BASE_MAX, "is_digit: base must be >=2 and <= BASE_MAX");
    for(size_t i = 0; i < base; i++)
    {
        if(character == ((capitalized) ? capital_digits[i] : lower_digits[i]))
        {
            return true;
        }
    }
    return false;
}


// Get the number of characters before the null terminator in a string.
size_t str_length(const char* const string)
{
    size_t string_size = 0;
    while(string[string_size] != '\0')
    {
        string_size++;
    }
    return string_size;
}

// Copy a string to a buffer
// Returns number of characters copied
size_t str_copy(const char* const to_copy, char* const copy_to, size_t copy_to_size)
{
    size_t to_copy_size = str_length(to_copy);
    size_t i = 0;
    for(; i < to_copy_size && i < copy_to_size && to_copy[i] != '\0'; i++)
    {
        copy_to[i] = to_copy[i];
    }
    return i; // Return number of chars written
}

void str_reverse(char* const str, const size_t str_size)
{
    char start, end;
    for(size_t i = 0; i < str_size / 2; i++)
    {
        // Store characters in the beginning and end of string.
        start = str[i];
        end = str[str_size - i - 1];
        
        // Put end char at start and start char at end.
        str[i] = end;
        str[str_size - i - 1] = start;
    }
}
