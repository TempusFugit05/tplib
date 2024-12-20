#include <unistd.h>
#include <stdarg.h>
#include <cstddef>
#include <cstdio>

#include "system_io.h"
#include "io.h"
#include "string_utils.h"
#include "math.h"
#include "system_params.h"
#include "dev_debug.h"

#define ARRAY_LENGTH(ARRAY) (sizeof(ARRAY) / sizeof(ARRAY[0]))
#define MODIFIER_CHAR '$'
#define PAREN_OPEN '{'
#define PAREN_CLOSE '}'
#define BASE_MAX 36

const char capital_digits[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    const char lower_digits[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};

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

// Check if a character is a digit.
bool is_digit(const char character, unsigned int base = 10, bool capitalized = false)
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

// Get a digit from a character.
int char_to_digit(const char character, unsigned int base = 10, bool capitalized = false)
{
    DEV_ASSERT(is_digit(character, base, capitalized), "char_to_num: The passed character is not a digit in the given base.");
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

struct str_num_struct
{
    int number;
    int status;
    int num_digits;
};

// Turn a string of digits to a number.
// search_whole - search the whole string for a number, else search only from the start of the string.
// status - -1 if no number found, otherwise offset from the start of the string where a number was found.
// num_digits - Number of digits found in the number returned.
str_num_struct str_to_num(const char* str, size_t str_size, const bool search_whole = false,  unsigned int base = 10, bool capitalized = false)
{
    DEV_ASSERT(base <= BASE_MAX, "is_digit: base must be >=2 and <= BASE_MAX");

    bool is_negative = false;

    if(str[0] == '-')
    {
        is_negative = true;
        str++;
        str_size--;
    }
    
    int number_start = 0;
    int digit_count = 0;
    int ret_number = 0;
    bool number_found = false;
    
    for(size_t i = 0; i < str_size; i++)
    {
        bool is_number = is_digit(str[i]);
        if(!is_number && number_found && !search_whole)
        {
            break;
        }
        else if(is_number && !number_found)
        {
            number_found = true;
            number_start = i;
        }
        if (is_number && number_found)
        {
            digit_count++;
        }
    }
    
    if(number_found)
    {
        int pwr = power(base, digit_count - 1);
        
        for(size_t i = number_start; (int)i < number_start + digit_count; i++)
        {
            int digit = char_to_digit(str[i], base, capitalized);
            ret_number += digit * pwr;
            pwr /= base;
        }

        if(is_negative)
        {
            ret_number *= -1;
        }
    }

    return 
    {
        ret_number,
        number_start,
        digit_count
    };
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

void str_write(const char* const string, int file)
{
    write_char_buffer(string, str_length(string), file);
}

void str_write(const char* const string, const size_t string_length, const int file)
{
    size_t size_to_terminator = str_length(string);
    write_char_buffer(string, ((string_length > size_to_terminator) ? size_to_terminator : string_length), file);
}

// Convert an integer to a string.
// Returns the number of digits written (including minus sign if negative).
size_t to_str(int integer, char* const buffer, const size_t buffer_size, const int base = 10, const bool capitalized = false, const bool big_endian = true, const char seperator = 0, const unsigned int seperate_every = -1)
{
    DEV_ASSERT(base >= 2 && base <= BASE_MAX, "to_str: Base must be greater than 1 and smaller than 37");
    const char* digits = (capitalized) ? capital_digits : lower_digits;
   
    size_t num_digits = 0;
    size_t num_chars = 0;
    bool seperator_added = false;
    
    long number; 
    if(big_endian && seperator == 0)
    {
        number = reverse(integer);
    }
    else
    {
        number = integer;
    }

    // https://math.stackexchange.com/a/111158
    if(number == 0)
    {
        buffer[0] = '0';
        return 1;
    } // This approach cannot handle a value of 0.

    if(number < 0)
    {
        buffer[0] = '-';
        
        // Calculate digits of the number and set string characters.
        for(; number < 0 && num_chars <= buffer_size - 1; num_chars++)
        {
            if(num_digits % seperate_every == 0 && num_digits != 0 && !seperator_added)
            {   
                seperator_added = true;
                buffer[num_chars + 1] = seperator;
            }
            else
            {
                buffer[num_chars + 1] = digits[-number % base];
                number /= base;
                num_digits++;
                seperator_added = false;
            }
        }
        if(!big_endian || seperator != 0)
        {
            str_reverse(buffer + 1, num_chars);
        }
        return num_chars + 1;
    }
    
    else
    {
        // Calculate digits of the number and set string characters.
        for(; number > 0 && num_chars <= buffer_size; num_chars++)
        {
            if(num_digits % seperate_every == 0 && num_digits != 0 && !seperator_added)
            {  
                seperator_added = true;
                buffer[num_chars] = seperator;
            }
            else
            {
                buffer[num_chars] = digits[number % base];
                number /= base;
                num_digits++;
                seperator_added=false;
            }
        }
        if(!big_endian || seperator != 0)
        {
            str_reverse(buffer, num_chars);
        }
        return num_chars;
    }
}

struct offsets_struct
{
    size_t buffer; // Number of chars written to the buffer.
    size_t format; // Number of chars read from the format.
};

struct int_format_settings
{
    int base = 10;
    bool capitalized = false;
    bool big_endian = true;
    char seperator = 0;
    unsigned int seperate_every = -1;
};

size_t format_base(const char* const format, size_t format_size, int_format_settings* const settings)
{
    size_t format_offset = 0;
    if(format[format_offset] == 'b' || format[format_offset] == 'B')
    {    
        if(format[format_offset] == 'B')
        {
            settings->capitalized = true;
        }
        format_offset++; // Move after modifier char.
        str_num_struct found_base = str_to_num(format + format_offset, format_size - format_offset); // Turn base to number.
        int new_base = found_base.number;
        
        if(found_base.status != -1 && new_base >= 2 && new_base <= BASE_MAX)
        {
            settings->base = new_base;
            format_offset += found_base.num_digits;
            if(settings->base == 2)
            {
                settings->big_endian = false;
            } // Base 2 is usually written with little endian (opposite of usual).
        } // Assign base if it is valid and move cursor after base number.
        else
        {
            return 0;
        }
    } 
    return format_offset;
}

size_t format_seperator(const char* const format, size_t format_size, int_format_settings* const settings)
{
    size_t format_offset = 0;
    if(format[format_offset] == 's')
    {    
        format_offset++; // Move after modifier char.
        if(format[format_offset] != '\0' && format_offset != format_size - 1)
        {
            settings->seperator = format[format_offset];
            format_offset++; // Move after seperator char.
            if(format[format_offset] == 'e')
            {
                format_offset++;
                str_num_struct seperator = str_to_num(format + format_offset, format_size - format_offset);
                if(seperator.status == -1)
                {
                    settings->seperate_every = 3;
                }
                else
                {
                    format_offset += seperator.num_digits;
                    settings->seperate_every = seperator.number;
                }
            }
        } // Check that we aren't hitting eol.
        else
        {
            return 0;
        }
    } 
    return format_offset;

}

#define FORMAT(FORMATTER, WAS_FORMATTED) \
if(!WAS_FORMATTED) \
        { \
            offset = FORMATTER(format_start, adjusted_format_size, &settings); \
            if(offset != 0) \
            { \
                WAS_FORMATTED = true; \
                format_offset += offset; \
                continue; \
            } \
        } \
// Part of str_format.
// This bit handles int formatting.
offsets_struct format_int(const char* const format, size_t format_size, char* const buffer, const size_t buffer_size, int to_format)
{
    size_t format_offset = 0;
    
    bool base_formatted = false;
    bool seperator_formatted = false;

    int_format_settings settings;
        
    while(format_offset < format_size)
    { 
        size_t offset = 0; // Offset of each format function
        const char* const format_start = format + format_offset;
        size_t adjusted_format_size = format_size - format_offset;

        FORMAT(format_base, base_formatted);
        FORMAT(format_seperator, seperator_formatted);

        if(offset == 0)
        {
            break;
        }
        format_offset += offset;
    }
        
    return {to_str(to_format, buffer, buffer_size, settings.base, settings.capitalized, settings.big_endian, settings.seperator, settings.seperate_every), format_offset};
}

// Return the index of the first occurance of a character in a string.
// If the character was not found, returns -1.
int find_char(const char* const str, const size_t str_length, const char to_find)
{
    for(size_t i = 0; i < str_length; i++)
    {
       if(str[i] == to_find)
       {
            return (int)i;
       }
    }
    return -1;
}

// Format a string.
// Returns number of characters formatted in the format buffer.
size_t str_format(const char* const format, char* const buffer, size_t buffer_size, va_list args)
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

// Print a formatted string to stdout
void str_print_formatted(const char* const format, ...)
{
    char buffer[256];
    const size_t format_size = str_length(format);
    size_t chars_formatted = 0;
    va_list args;
    va_start(args, format);
    while(chars_formatted < format_size)
    {
        chars_formatted += str_format(format + chars_formatted, buffer, ARRAY_LENGTH(buffer), args);
        str_write(buffer, ARRAY_LENGTH(buffer));
    }
    va_end(args);
}
