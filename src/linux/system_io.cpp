#include <unistd.h>
#include "system_io.h"

void file_write(const char* const buffer, const size_t buffer_size, const int file)
{
    write(file, buffer, buffer_size);
}

void file_read(char* buffer, const size_t buffer_size, const int file)
{
    read(file, buffer, buffer_size);
}

void std_read(char* buffer, const size_t buffer_size)
{
    file_read(buffer, buffer_size, 0);
}
