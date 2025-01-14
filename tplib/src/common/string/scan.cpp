#include <cstddef>
#include <cstdio>
#include "dev_debug.h"
#include "math.h"
#include "scan.h"

str_num_struct str_to_num(const char* str, size_t str_size, const bool search_whole,  unsigned int base, bool capitalized)
{
    DEV_ASSERT(base <= BASE_MAX, "is_digit: base must be >=2 and <= BASE_MAX");

    bool is_negative = false;
    
    if(!search_whole && str[0] == '-')
    {
        is_negative = true;
        str++;
        str_size--;
    } // Accounting for negative values.

    int number_start = 0;
    int digit_count = 0;
    bool number_found = false;
    
    for(size_t i = 0; i < str_size; i++)
    { 
        if(is_digit(str[i], base, capitalized))
        {
            if(!number_found)
            {
                number_found = true;
                number_start = i;
            } // When we found the start of a number.
            digit_count++;
        }
        else if(!search_whole)
        {
            break;
        } // Stop search if we found and reached the end of a number.
        else if(number_found)
        {
            break;
        }
    }
    
    if(search_whole && !is_negative && number_start > 0)
    {
        if(str[number_start - 1] == '-')
        {
            is_negative = true;
        }
    } // Check if number is negative.

    long ret_number = 0;
    if(number_found)
    {
        long pwr = power((long)base, digit_count - 1);
        for(int i = 0; i <= digit_count; i++)
        {
            int digit = char_to_digit(str[number_start + i], capitalized);
            ret_number += digit * pwr;
            pwr /= base;
        } // Converting the sequence of digits into numbers in the given base.
        
        //DEV_ASSERT((is_negative) ? ret_number > 0 :ret_number < 0, "srt_to_num integer underflow/overflow detected.");

        if(is_negative)
        {
            ret_number *= -1;
        }
    }
    return
    {
        (int)ret_number,
        number_start,
        digit_count
    };
}


