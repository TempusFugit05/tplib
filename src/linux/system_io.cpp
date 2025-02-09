#include <unistd.h>
#include "system_io.h"

// Write to file
void file_write(const char* const buffer, const size_t buffer_size, const int file)
{
    write(file, buffer, buffer_size);
}

// Read file
int file_read(char* buffer, const size_t buffer_size, const int file)
{
    return read(file, buffer, buffer_size);
}

