#include <unistd.h>
#include "system_io.h"

void write_char_buffer(const char* const buffer, const size_t buffer_size, const int file)
{
    write(file, buffer, buffer_size);
}

void write_char(const char character, const int file)
{
    const char char_to_write[1] = {character};
    write(file, char_to_write, 1);
}

void file_read(char* buffer, const size_t buffer_size, const int file)
{
    read(file, buffer, buffer_size);
}

void std_read(char* buffer, const size_t buffer_size)
{
    file_read(buffer, buffer_size, 0);
}
