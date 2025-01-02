#include <cstddef>
#include <cstdio>

#include "math.h"
#include "format.h"
#include "string.h"
#include "dev_debug.h"

// Convert an integer to a string.
// Returns the number of digits written (including minus sign if negative).
size_t to_str(int integer, char* const buffer, const size_t buffer_size, const int base, const bool capitalized, const bool big_endian, const char seperator, const unsigned int seperate_every)
{
    DEV_ASSERT(base >= 2 && base <= BASE_MAX, "to_str: Base must be greater than 1 and smaller than 37");
    const char* digits = (capitalized) ? capital_digits : lower_digits;
   
    size_t num_digits = 0;
    size_t num_chars = 0;
    bool seperator_added = false;
    
    long number; 
    if(big_endian && seperator == 0)
    {
        number = reverse(integer);
    }
    else
    {
        number = integer;
    }

    // https://math.stackexchange.com/a/111158
    if(number == 0)
    {
        buffer[0] = '0';
        return 1;
    } // This approach cannot handle a value of 0.

    if(number < 0)
    {
        buffer[0] = '-';
        
        // Calculate digits of the number and set string characters.
        for(; number < 0 && num_chars <= buffer_size - 1; num_chars++)
        {
            if(num_digits % seperate_every == 0 && num_digits != 0 && !seperator_added)
            {   
                seperator_added = true;
                buffer[num_chars + 1] = seperator;
            }
            else
            {
                buffer[num_chars + 1] = digits[-number % base];
                number /= base;
                num_digits++;
                seperator_added = false;
            }
        }
        if(!big_endian || seperator != 0)
        {
            str_reverse(buffer + 1, num_chars);
        }
        return num_chars + 1;
    }
    else
    {
        // Calculate digits of the number and set string characters.
        for(; number > 0 && num_chars <= buffer_size; num_chars++)
        {
            if(num_digits % seperate_every == 0 && num_digits != 0 && !seperator_added)
            {  
                seperator_added = true;
                buffer[num_chars] = seperator;
            }
            else
            {
                buffer[num_chars] = digits[number % base];
                number /= base;
                num_digits++;
                seperator_added=false;
            }
        }
        if(!big_endian || seperator != 0)
        {
            str_reverse(buffer, num_chars);
        }
        return num_chars;
    }
}
