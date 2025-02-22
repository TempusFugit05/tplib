#ifndef STRING_TYPES_H
#define STRING_TYPES_H
#include <cstddef>

#include "tp_string.h"

namespace tp
{
    // This class manages a char buffer and adds functionality similar to stdout, but without printing to the console.
    template <int size> class static_string_t
    {
        private:
            size_t buffer_size = size - 1; // Size of the char buffer. 1 byte is reserved for terminator.
            char buffer [size];
            size_t cursor = 0; // Indicates the end of the string
        
        private:
            // Returns the remaining space in the internal buffer.
            size_t remaining_space()
            {
                return buffer_size - cursor - 1;
            }
    
            // Returns a pointer to the next character in the string.
            char* buffer_offset()
            {
                return buffer + cursor;
            }
    
        public:    
            // Returns a read-only pointer to the internal buffer.
            const char* get_string()
            {
                buffer[cursor] = '\0'; // Add null terminator to end of string.
                return buffer;
            }
    
            // Append a null-terminated char buffer to this string.
            static_string_t& operator+(const char* const string)
            {
                cursor += str_copy(string, buffer_offset(), remaining_space());
                return *this;
            }
    
            // Append an integer to this string.
            static_string_t& operator+(const int number)
            { 
                cursor += to_str(number, buffer_offset(), remaining_space());
                return *this;
            }
    
            // Move cursor back by the given number.
            // Does nothing if a negative number or 0 is passed.
            static_string_t& operator-(const int to_remove)
            {
                if(to_remove > 0)
                {
                    cursor -= ((size_t)to_remove < cursor) ? to_remove : cursor;
                }
                return *this;
            }
        };            
}
#endif // STRING_TYPES_H
