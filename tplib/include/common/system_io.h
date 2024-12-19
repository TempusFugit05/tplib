#ifndef SYSTEM_IO_H
#define SYSTEM_IO_H

// Returns the number of characters until a null terminator symbol.
size_t get_string_length(const char* const string);

void write_char_buffer(const char* const buffer, const size_t buffer_size, const int file = 1);

void write_char(const char character, const int file = 1);
#endif // SYSTEM_IO_H
