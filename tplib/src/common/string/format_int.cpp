#include <cstddef>
#include <stdarg.h>

#include "string.h"
#include "string_utils.h"
#include "macro_utils.h"

#define MODIFIER_CHAR '$'
#define PAREN_OPEN '{'
#define PAREN_CLOSE '}'


struct int_format_settings
{
    int base = 10;
    bool capitalized = false;
    bool big_endian = true;
    
    char seperator = 0;
    int seperate_every = -1;
    
    int fill = 0;
    char fill_with = '0';
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
                } // Default to seperating every 3 digits.
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


size_t format_fill(const char* const format, size_t format_size, int_format_settings* const settings)
{
    size_t format_offset = 0;
    if(format[format_offset] == 'f')
    {
        format_offset++;
        str_num_struct found_number = str_to_num(format + format_offset,
                                                format_size - format_offset); // Search for number of chars to fill
        if(found_number.status == -1)
        {
            return 0;
        } // If no number found, return
        format_offset += found_number.num_digits;
        settings->fill = found_number.number;
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
    bool fill_formatted = false;

    int_format_settings settings;
        
    while(format_offset < format_size)
    { 
        size_t offset = 0; // Offset of each format function
        const char* const format_start = format + format_offset;
        size_t adjusted_format_size = format_size - format_offset;

        FORMAT(format_base, base_formatted);
        FORMAT(format_seperator, seperator_formatted);
        FORMAT(format_fill, fill_formatted);

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


