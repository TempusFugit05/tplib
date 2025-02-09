#include <cstddef>

#include <termios.h>

#include "io.h"
#include "system_io.h"
#include "console_io.h"
#include "system_files.h"
#include "macro_utils.h"

// Read input from stdin
int console_read(char* buffer, const size_t buffer_size)
{
    int bytes_read = file_read(buffer, buffer_size - 1, FILE_STDIN); // Read bytes while reserving space for terminator.
    size_t string_end = 0; // Index to which write terminator.
    if(bytes_read > 0)
    {
        string_end = bytes_read;
        tcflush(FILE_STDIN, TCIFLUSH);
    }
    
    buffer[string_end] = '\0';
    return bytes_read;
}
