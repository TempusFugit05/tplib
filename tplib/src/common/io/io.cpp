#include <stdarg.h>
#include <cstddef>

#include "system_params.h"
#include "system_io.h"

#include "io.h"
#include "string_utils.h"
#include "math.h"
#include "dev_debug.h"
#include "macro_utils.h"

void str_write(const char* const string, int file)
{
    write_char_buffer(string, str_length(string), file);
}

void str_write(const char* const string, const size_t string_length, const int file)
{
    size_t size_to_terminator = str_length(string);
    write_char_buffer(string, ((string_length > size_to_terminator) ? size_to_terminator : string_length), file);
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
        str_write(buffer, ARRAY_LENGTH(buffer), FILE_STDOUT);
    }
    va_end(args);
}
