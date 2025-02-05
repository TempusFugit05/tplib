#ifndef SYSTEM_IO_H
#define SYSTEM_IO_H
void write_char_buffer(const char* const buffer, const size_t buffer_size, const int file);
void write_char(const char character, const int file);
void file_read(char* buffer, const size_t buffer_size, const int file);
void std_read(char* buffer, const size_t buffer_size);
#endif // SYSTEM_IO_H
