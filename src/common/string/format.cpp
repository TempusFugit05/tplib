#include <cstddef>
#include <cstdio>

#include "math.h"
#include "format.h"
#include "format_utils.h"
#include "tp_string.h"
#include "dev_debug.h"

#define MODIFIER_CHAR '$'
#define PAREN_OPEN '{'
#define PAREN_CLOSE '}'

// Convert an integer to a string.
// Returns the number of digits written (including minus sign if negative).
size_t to_str(int integer, char* const buffer, const size_t buffer_size, const int base, const bool capitalized, const char seperator, const unsigned int seperate_every)
{
    DEV_ASSERT(base >= 2 && base <= BASE_MAX, "to_str: Base must be greater than 1 and smaller than 37");
    const char* digits = (capitalized) ? capital_digits : lower_digits;
   
    size_t num_digits = 0;
    size_t num_chars = 0;

    bool seperator_added = false;
    
    long number = integer; 

    // https://math.stackexchange.com/a/111158
    if(number == 0)
    {
        buffer[0] = '0';
        return 1;
    } // This approach cannot handle a value of 0.

    int offset = 0;
    if(number < 0)
    {
        buffer[0] = '-';
        offset = 1;
    }
    
    // Calculate digits of the number and set string characters.
    for(; ((integer < 0) ? number < 0 : number > 0) && num_chars <= buffer_size - offset; num_chars++)
    {
        if(num_digits % seperate_every == 0 && num_digits != 0 && !seperator_added)
        {   
            seperator_added = true;
            buffer[num_chars + offset] = seperator;
        }
        else
        {
            buffer[num_chars + offset] = digits[((offset == 1) ? -number : number) % base];
            number /= base;
            num_digits++;
            seperator_added = false;
        }
    }
    str_reverse(buffer + offset, num_chars);
    return num_chars + offset;
}

// Format a string.
// Returns number of characters formatted in the format buffer.
size_t str_format_internal(char* const buffer, size_t buffer_size, const char* const format, va_list args)
{
    unsigned int buffer_ptr = 0;
    bool format_next = false;
    const size_t format_length = str_length(format);

    buffer_size--; // Reserve a byte for null terminator
    size_t i = 0;
    
    for(; i < format_length && buffer_ptr < buffer_size; i++)
    {
        if(format_next)
        {
            char* const buff_start = buffer + buffer_ptr;
            const size_t chars_remaining = buffer_size - buffer_ptr;
            
            const char* const format_start = format + i + 1;
            const size_t format_chars_remaining = format_length - i;
            
            offsets_struct offset = {0, 0};

            switch(format[i])
            {
                case 'i':
                {
                    // Write number to buffer and offset the buffer pointer by the number of chars written.
                    offset = format_int(format_start, format_chars_remaining, buff_start,
                                        chars_remaining, va_arg(args, int)); 
                    break;
                }
                case 's':
                {
                    // Copy string to buffer abd offset buffer pointer by number of chars written
                    offset.buffer = str_copy(va_arg(args, char*), buff_start, chars_remaining);
                    break; 
                }
                case 'c':
                {
                    // Copy a character to the buffer
                    buff_start[0] = (char)va_arg(args, int);
                    offset.buffer = 1;
                    break;
                }
            }
            
            buffer_ptr += offset.buffer;
            i += offset.format;
            format_next = false;
        } // Format the values into strings and write them to the buffer.
        else if(format[i] == MODIFIER_CHAR)
        { 
            format_next = true;
        } // If a modifier char is found, format an argument to a string on next iteration.
        else
        {
            buffer[buffer_ptr] = format[i];
            buffer_ptr++;
        } // If no modifiers found, write the current char to the buffer.
    }
    if(buffer_size != 0)
    {
        buffer[buffer_ptr] = '\0'; // Add null terminator to end of resulting string
    }
    return i;
}
