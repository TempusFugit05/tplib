#ifndef MANIPULATE_H
#define MANIPULATE_H

int char_to_digit(const char character, bool capitalized = false);
bool is_digit(const char character, unsigned int base = 10, bool capitalized = false);
size_t str_length(const char* const string);
size_t str_copy(const char* const to_copy, const size_t to_copy_size, char* const copy_to, const size_t copy_to_size);
size_t str_copy(const char* const to_copy, char* const copy_to, size_t copy_to_size);
void str_reverse(char* const str, const size_t str_size);

#endif // MANIPULATE_H
