#ifndef SYSTEM_IO_H
#define SYSTEM_IO_H
void file_write(const char* const buffer, const size_t buffer_size, const int file);
int file_read(char* buffer, const size_t buffer_size, const int file);
#endif // SYSTEM_IO_H
