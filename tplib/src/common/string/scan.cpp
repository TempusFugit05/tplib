#include "scan.h"

#include <cstddef>

#include "dev_debug.h"
#include "math.h"

str_num_struct str_to_num(const char* str, size_t str_size, const bool search_whole,  unsigned int base, bool capitalized)
{
    DEV_ASSERT(base <= BASE_MAX, "is_digit: base must be >=2 and <= BASE_MAX");

    bool is_negative = false;

    if(str[0] == '-')
    {
        is_negative = true;
        str++;
        str_size--;
    }
    
    int number_start = 0;
    int digit_count = 0;
    int ret_number = 0;
    bool number_found = false;
    
    for(size_t i = 0; i < str_size; i++)
    {
        bool is_number = is_digit(str[i]);
        if(!is_number && number_found && !search_whole)
        {
            break;
        }
        else if(is_number && !number_found)
        {
            number_found = true;
            number_start = i;
        }
        if (is_number && number_found)
        {
            digit_count++;
        }
    }
    
    if(number_found)
    {
        int pwr = power(base, digit_count - 1);
        
        for(size_t i = number_start; (int)i < number_start + digit_count; i++)
        {
            int digit = char_to_digit(str[i], capitalized);
            ret_number += digit * pwr;
            pwr /= base;
        }

        if(is_negative)
        {
            ret_number *= -1;
        }
    }

    return 
    {
        (is_negative) ? -ret_number : ret_number,
        number_start,
        digit_count
    };
}


