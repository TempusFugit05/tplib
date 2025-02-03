#include <cstddef>
#include <stdarg.h>
#include <cstdio>

#include "tp_string.h"
#include "string_utils.h"
#include "macro_utils.h"

struct int_format_settings
{
    int base = 10;
    bool capitalized = false;
    
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
        str_num_struct found_base = str_to_num(
        format + format_offset,
        format_size - format_offset,
        false,
        10,
        false); // Turn base to number.
        int new_base = found_base.number;
        if(found_base.status != -1 && new_base >= 2 && new_base <= BASE_MAX)
        {
            settings->base = new_base;
            format_offset += found_base.num_digits;
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
    size_t num_chars =
        to_str(
        to_format,
        buffer,
        buffer_size,
        settings.base,
        settings.capitalized,
        settings.seperator,
        settings.seperate_every
        );
             
    return {num_chars ,format_offset};
}


