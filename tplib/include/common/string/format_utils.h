#ifndef FORMAT_UTILS_H
#define FORMAT_UTILS

size_t str_format_internal(char* const buffer, size_t buffer_size, const char* const format, va_list args);

#endif // FORMAT_UTILS
